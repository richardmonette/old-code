
class VolumeRegion
{
public:

	virtual BBox WorldBound() const = 0;

	virtual bool IntersectP(const Ray &ray, float *t0, float *t1) const = 0;

	virtual Spectrum sigma_a(const Point &, const Vector &) const = 0;
	virtual Spectrum sigma_s(const Point &, const Vector &) const = 0;
	virtual Spectrum Lve(const Point &, const Vector &) const = 0;
	virtual float p(const Point &, const Vector &, const Vector &) const = 0;

	virtual Spectrum Tau(const Ray &ray, float step = 1.f, float offset = 0.5) const = 0;

	Spectrum sigma_t(const Point &p, const Vector &w) const
	{
		return sigma_a(p,w) + sigma_s(p,w);
	}
};