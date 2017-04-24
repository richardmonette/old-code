
#include "Vector3.h"

const float PI = 3.1415926535897932384626433832795;
const float PIdiv180 = (PI/180.0);

class CCamera
{
private:
	
	Vector3 ViewDir;
	Vector3 RightVector;	
	Vector3 UpVector;
	Vector3 Position;

	float rotX, rotY;

	Vector3 unit_x, unit_y, unit_z;
	
public:

	CCamera();				
	void Render();

	Vector3 GetPosition() { return Position; }
	Vector3 GetView() { return ViewDir; }
	Vector3 GetUp() { return UpVector; }

	void Move( Vector3 Direction );
	
	void SetViewByMouse(float x, float y);

	void MoveForward( float Distance );
	void MoveUpward( float Distance );
	void StrafeRight( float Distance );
};


