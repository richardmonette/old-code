
// Date: Friday November 19th, 2010

// Description: GPU implementation of Jos Stam's GDC '03 paper "Real-time Fluid Dynamics For Games" using NVidia CUDA

// Author: Richard E. Monette

#include "../common/book.h"
#include "../common/gpu_anim.h"

#define DIM 512
#define DT 0.1f
#define DIFFUSION 0.0f
#define VISCOSITY 0.0f
#define FORCE 5.0f
#define SOURCE 10.0f

static float *u;
static float *v;
static float *u_prev;
static float *v_prev;
static float *dens;
static float *dens_prev;

texture<float> tex_ref;
texture<float> tex_ref2;
texture<float> tex_ref3;

static float *draw_buffer_dens;
static float *draw_buffer_u;
static float *draw_buffer_v;

static int win_id;
static int mouse_down[3];
static int omx, omy, mx, my;

dim3    blocks(DIM/16,DIM/16);
dim3    threads(16,16);

#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}
#define IX(i,j) ((i)+(DIM)*(j))

__global__ void add_force(int pixel, float u_value, float v_value, float *uu, float *vv)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

	if (abs(offset - pixel) < 3)
	{
		uu[offset] = u_value;
		vv[offset] = v_value;
	}
}

__global__ void set_density(int pixel, float value, float *dens)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

	if (abs(offset - pixel) < 3)
	{
		dens[offset] = value;
	}
}

__global__ void add_source(float *t, float *s) 
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

    t[offset] += DT * s[offset];
}

__global__ void clear_kernel(float *t) 
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

    t[offset] = 0.0f;
}

__global__ void set_bnd_kernel_0(float *x)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	if (i == 0) x[IX(  0, i  )] = tex1D(tex_ref, IX(1,i));
	if (i == DIM-1) x[IX(DIM-1, i  )] = tex1D(tex_ref, IX(DIM-2,i));

	if (j == 0) x[IX(  i, 0  )] = tex1D(tex_ref, IX(i,1));
	if (j == DIM-1) x[IX(  i, DIM-1)] = tex1D(tex_ref, IX(i,DIM-2));
}

__global__ void set_bnd_kernel_1(float *x)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	if (i == 0) x[IX(  0, i  )] = -tex1D(tex_ref, IX(1,i));
	if (i == DIM-1) x[IX(DIM-1, i  )] = -tex1D(tex_ref, IX(DIM-2,i));
}

__global__ void set_bnd_kernel_2(float *x)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	if (j == 0) x[IX(  i, 0  )] = -tex1D(tex_ref, IX(i,1));
	if (j == DIM-1) x[IX(  i, DIM-1)] = -tex1D(tex_ref, IX(i,DIM-2));
}

__global__ void advect(float *d, float *d0, float *u, float *v) 
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
	int offset = i + j * blockDim.x * gridDim.x;

	int left = offset - 1;
	int right = offset + 1;
	if (i == 0) left++;
	if (i == DIM - 1) right--;

	int top = offset - DIM;
	int bottom = offset + DIM;
	if (j == 0) top += DIM;
	if (j == DIM - 1) bottom -= DIM;

	int i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = (float)DT * DIM;

	x = i - dt0 * tex1Dfetch(tex_ref2, offset);
	y = j - dt0 * tex1Dfetch(tex_ref3, offset);

	if (x < 0.5f) x = 0.5f;
	if (x > DIM + 0.5f) x = DIM + 0.5f;
	i0 = (int)x;
	i1 = i0 + 1;
	
	if (y < 0.5f) y = 0.5f;
	if (y > DIM + 0.5f) y = DIM + 0.5f;
	j0 = (int)y;
	j1 = j0 + 1;

	s1 = x - i0;
	s0 = 1 - s1;
	t1 = y - j0;
	t0 = 1 - t1;

	if (i0 < 0) i0 = 0;
	if (i1 > DIM - 1) i1 = DIM - 1;

	if (j0 < 0) j0 = 0;
	if (j1 > DIM - 1) j1 = DIM - 1;

	d[offset] = s0 * (t0 * tex1Dfetch(tex_ref, i0 + j0 * DIM) + t1 * tex1Dfetch(tex_ref, i0 + j1 * DIM)) + 
		        s1 * (t0 * tex1Dfetch(tex_ref, i1 + j0 * DIM) + t1 * tex1Dfetch(tex_ref, i1 + j1 * DIM));
}

__global__ void diffuse_kernel(float *x, float *x0) 
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
	int offset = i + j * blockDim.x * gridDim.x;

	int left = offset - 1;
	int right = offset + 1;
	if (i == 0) left++;
	if (i == DIM - 1) right--;

	int top = offset - DIM;
	int bottom = offset + DIM;
	if (j == 0) top += DIM;
	if (j == DIM - 1) bottom -= DIM;

	const float a = (float)DT * (float)DIFFUSION * DIM * DIM;

	x[offset] = x0[offset] + a * (x[left] + x[right] + x[bottom] + x[top]) / (1 + 4 * a);
}

__global__ void project_kernel1(float *u, float *v, float *p, float *div, float h) 
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	div[IX(i,j)] = -0.5f*(u[IX(i+1,j)]-u[IX(i-1,j)]+v[IX(i,j+1)]-v[IX(i,j-1)])/DIM;
	p[IX(i,j)] = 0;
}

__global__ void project_kernel2(float * x, float * x0, float a, float c) 
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	x[IX(i,j)] = (  tex1Dfetch(tex_ref2, IX(i,j)) + a * (  tex1Dfetch(tex_ref, IX(i-1,j)) + tex1Dfetch(tex_ref, IX(i+1,j)) + tex1Dfetch(tex_ref, IX(i,j-1)) + tex1Dfetch(tex_ref, IX(i,j+1))   )  )/c;
}

__global__ void project_kernel3(float *u, float *v, float *p, float h) 
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;

	u[IX(i,j)] -= 0.5f * DIM * (tex1Dfetch(tex_ref, IX(i+1,j)) - tex1Dfetch(tex_ref, IX(i-1,j)));
	
	v[IX(i,j)] -= 0.5f * DIM * (tex1Dfetch(tex_ref, IX(i,j+1)) - tex1Dfetch(tex_ref, IX(i,j-1)));
}

void diffuse(float *x, float *x0) 
{
	for (int k = 0; k < 20; k++)
	{
		diffuse_kernel<<<blocks, threads>>>(x, x0);
		
		cudaBindTexture(NULL, tex_ref, x, DIM * DIM * sizeof(float));
		set_bnd_kernel_0<<<blocks, threads>>>(x);
	}
}

void project(float *u, float *v, float *p, float *div)
{
	float h = 1.0 / DIM;

	project_kernel1<<<blocks, threads>>>(u, v, p, div, h);

	cudaBindTexture(NULL, tex_ref, div, DIM * DIM * sizeof(float));
	set_bnd_kernel_0<<<blocks, threads>>>(div); 

	cudaBindTexture(NULL, tex_ref, p, DIM * DIM * sizeof(float));
	set_bnd_kernel_0<<<blocks, threads>>>(p);

	for (int k = 0; k < 20; k++)
	{
		cudaBindTexture(NULL, tex_ref, p, DIM * DIM * sizeof(float));
		cudaBindTexture(NULL, tex_ref2, div, DIM * DIM * sizeof(float));
		project_kernel2<<<blocks, threads>>>(p, div, 1, 4);

		cudaBindTexture(NULL, tex_ref, p, DIM * DIM * sizeof(float));
		set_bnd_kernel_0<<<blocks, threads>>>(p);
	}

	cudaBindTexture(NULL, tex_ref, p, DIM * DIM * sizeof(float));
	project_kernel3<<<blocks, threads>>>(u, v, p, h);

	cudaBindTexture(NULL, tex_ref, u, DIM * DIM * sizeof(float));
	set_bnd_kernel_1<<<blocks, threads>>>(u);

	cudaBindTexture(NULL, tex_ref, v, DIM * DIM * sizeof(float));
	set_bnd_kernel_2<<<blocks, threads>>>(v);
}

void dens_step(float *x, float *x0, float *u, float *v)
{
	add_source<<<blocks, threads>>>(x, x0);
	SWAP(x0, x);
	diffuse(x, x0);
	SWAP(x0, x);

	cudaBindTexture(NULL, tex_ref, x0, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref2, u, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref3, v, DIM * DIM * sizeof(float));
	advect<<<blocks, threads>>>(x, x0, u, v);

	cudaBindTexture(NULL, tex_ref, x, DIM * DIM * sizeof(float));
	set_bnd_kernel_0<<<blocks, threads>>>(x);
}

void vel_step(float *u, float *v, float *u0, float *v0)
{
	add_source<<<blocks, threads>>>(u, u0);
	add_source<<<blocks, threads>>>(v, v0);
	SWAP(u0, u);
	diffuse(u, u0);
	SWAP(v0, v);
	diffuse(v, v0);
	project(u, v, u0, v0);
	SWAP(u0, u);
	SWAP(v0, v);

	cudaBindTexture(NULL, tex_ref, u0, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref2, u0, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref3, v0, DIM * DIM * sizeof(float));
	advect<<<blocks, threads>>>(u, u0, u0, v0);

	cudaBindTexture(NULL, tex_ref, u, DIM * DIM * sizeof(float));
	set_bnd_kernel_1<<<blocks, threads>>>(u);

	cudaBindTexture(NULL, tex_ref, v0, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref2, u0, DIM * DIM * sizeof(float));
	cudaBindTexture(NULL, tex_ref3, v0, DIM * DIM * sizeof(float));
	advect<<<blocks, threads>>>(v, v0, u0, v0);

	cudaBindTexture(NULL, tex_ref, v, DIM * DIM * sizeof(float));
	set_bnd_kernel_2<<<blocks, threads>>>(v);

	project(u, v, u0, v0);
}

static void get_from_UI ( float * dd, float * uu, float * vv )
{
	clear_kernel<<<blocks, threads>>>(uu);
	clear_kernel<<<blocks, threads>>>(vv);
	clear_kernel<<<blocks, threads>>>(dd);

	if ( !mouse_down[0] && !mouse_down[2] ) return;

	int i = (int)mx;
	int j = (int)DIM - my;

	if ( i < 1 || i > DIM || j < 1 || j > DIM ) return;

	if ( mouse_down[0] ) 
	{
		add_force<<<blocks, threads>>>(i + j * DIM, FORCE * (mx-omx), FORCE * (omy-my), uu, vv);
	}

	if ( mouse_down[2] ) 
	{
		set_density<<<blocks, threads>>>(i + j * DIM, SOURCE, dd);
	}

	omx = mx;
	omy = my;

	return;
}

void simulate() 
{
	get_from_UI ( dens_prev, u_prev, v_prev );
    vel_step(u, v, u_prev, v_prev);
	dens_step(dens, dens_prev, u, v);
}

static void pre_display ( void )
{
	glViewport( 0, 0, DIM, DIM );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

static void post_display ( void )
{
	glutSwapBuffers();
}

static void idle_func ( void )
{
	simulate();

	glutSetWindow(win_id);
	glutPostRedisplay();
}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) 
	{
		*r = *g = *b = v;
		return;
	}
	h /= 60;
	i = floor( h );
	f = h - i;
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

static void display_func ( void )
{
	pre_display();

		cudaMemcpy(draw_buffer_dens, dens, DIM * DIM * sizeof(float), cudaMemcpyDeviceToHost);

		float d00, d01, d10, d11;
		float r, g, b;

		glBegin ( GL_QUADS );

			for (int i = 0; i < DIM; i++) 
			{
				for (int j = 0; j < DIM; j++) 
				{
					d00 = draw_buffer_dens[i + j * DIM];
					d01 = draw_buffer_dens[i + (j+1) * DIM];
					d10 = draw_buffer_dens[(i+1) + j * DIM];
					d11 = draw_buffer_dens[(i+1) + (j+1) * DIM];

					float x = i / DIM.f;
					float y = j / DIM.f;

					float h = 1.f / DIM.f;
					
					HSVtoRGB(&r, &g, &b, d00 * 360.f, 1, d00);
					glColor3f ( r, g, b ); glVertex2f ( x, y );
					
					HSVtoRGB(&r, &g, &b, d01 * 360.f, 1, d01);
					glColor3f ( r, g, b ); glVertex2f ( x+h, y );
					
					HSVtoRGB(&r, &g, &b, d10 * 360.f, 1, d10);
					glColor3f ( r, g, b ); glVertex2f ( x+h, y+h );
					
					HSVtoRGB(&r, &g, &b, d11 * 360.f, 1, d11);
					glColor3f ( r, g, b ); glVertex2f ( x, y+h );
				}
			}

		glEnd ();

	post_display();
}

static void reshape_func ( int width, int height )
{
	glutSetWindow ( win_id );
	glutReshapeWindow ( width, height );
}

static void mouse_func ( int button, int state, int x, int y )
{
	omx = mx = x;
	omx = my = y;

	mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func ( int x, int y )
{
	mx = x;
	my = y;
}

static void open_glut_window ( void )
{
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(DIM, DIM);
	win_id = glutCreateWindow("CUDA Fluid Simulation");

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers();
	glClear( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	pre_display();

	//glutKeyboardFunc ( key_func );
	glutMouseFunc ( mouse_func );
	glutMotionFunc ( motion_func );
	glutReshapeFunc ( reshape_func );
	glutIdleFunc ( idle_func );
	glutDisplayFunc ( display_func );
}

int main(int argc, char ** argv) 
{
	cudaMalloc((void**)&u, DIM * DIM * sizeof(float));
	cudaMalloc((void**)&v, DIM * DIM * sizeof(float));
	cudaMalloc((void**)&u_prev, DIM * DIM * sizeof(float));
	cudaMalloc((void**)&v_prev, DIM * DIM * sizeof(float));
	cudaMalloc((void**)&dens, DIM * DIM * sizeof(float));
	cudaMalloc((void**)&dens_prev, DIM * DIM * sizeof(float));

	draw_buffer_dens = (float*)malloc(DIM * DIM * sizeof(float));
	draw_buffer_u = (float*)malloc(DIM * DIM * sizeof(float));
	draw_buffer_v = (float*)malloc(DIM * DIM * sizeof(float));

	dim3    blocks(DIM/16,DIM/16);
    dim3    threads(16,16);

	clear_kernel<<<blocks, threads>>>(u);
	clear_kernel<<<blocks, threads>>>(v);
	clear_kernel<<<blocks, threads>>>(u_prev);
	clear_kernel<<<blocks, threads>>>(v_prev);
	clear_kernel<<<blocks, threads>>>(dens);
	clear_kernel<<<blocks, threads>>>(dens_prev);

	glutInit(&argc, argv);

	open_glut_window();

	glutMainLoop();

	cudaFree(u);
	cudaFree(v);
	cudaFree(u_prev);
	cudaFree(v_prev);
	cudaFree(dens);
	cudaFree(dens_prev);

	cudaUnbindTexture(tex_ref);

	free(draw_buffer_dens);
	free(draw_buffer_u);
	free(draw_buffer_v);
}
