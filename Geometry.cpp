#include "Geometry.h"


bool Plane::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{
  surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

  if(surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normal;
    surf.m_ptr    = m_ptr;
	surf.u = surf.hitPoint.x / 10;
	surf.v = surf.hitPoint.z / 10;
    return true;
  }

  return false;
}

/////////////////////////////////////////


//Для пересечения со сферой подставляем луч в уравнение сферы и решаем получившееся квадратное уравнение
//Решения этого уравнения - точки пересечения луча со сферой
bool Sphere::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{

  float3 k = ray.o - center;

  float a = dot(ray.d, ray.d);
  float b = dot(2 * k, ray.d);
  float c = dot(k, k) - r_sq;

  float d = b * b - 4 * a * c;

  if (d < 0) return false;

  surf.t = (-b - sqrt(d)) / 2 * a;

  if (surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normalize(surf.hitPoint - center);
    surf.m_ptr    = m_ptr;

	//normalize(surf.hitPoint);
	//float phi = acosf(-surf.hitPoint.z );

	//float theta = acosf (surf.hitPoint.y/sinf(phi)) / (2* PI);
	//surf.u = surf.hitPoint.x > 0 ? theta : 1 - theta;
	//surf.v = 1 - phi/PI;

	//float3 n = normalize(surf.hitPoint - center);
	//surf.u = atan2(n.x, n.z) / (2 * PI) + 0.5;
	//surf.v = n.y * 0.5 + 0.5;
	surf.u = asin(surf.normal.x) / PI + 0.5;
	surf.v = asin(surf.normal.y) / PI + 0.5;
    return true;
  }

  surf.t = (-b + sqrt(d)) / 2 * a;
  if (surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normalize(surf.hitPoint - center);
    surf.m_ptr    = m_ptr;
	float phi = atan2(surf.hitPoint.z, surf.hitPoint.x);
	float theta = asin(surf.hitPoint.y);
	float pi = 3.14159265358979323846;
	surf.u = 1 - (phi + pi) / (2 * pi);
	surf.v = (theta + pi / 2) / pi;
    return true;
  }

  return false;
}

bool Parallel::Intersect(const Ray &ray, float tmin, float tmax, SurfHit &surf) const
{
	float dirfracX = 1.0f / ray.d.x;
	float dirfracY = 1.0f / ray.d.y;
	float dirfracZ = 1.0f / ray.d.z;
	float x1 = (t_min.x - ray.o.x)*dirfracX;
	float x2 = (t_max.x - ray.o.x)*dirfracX;
	float y1 = (t_min.y - ray.o.y)*dirfracY;
	float y2 = (t_max.y - ray.o.y)*dirfracY;
	float z1 = (t_min.z - ray.o.z)*dirfracZ;
	float z2 = (t_max.z - ray.o.z)*dirfracZ;

	float tMin = max(max(min(x1, x2), min(y1, y2)), min(z1, z2));
	float tMax = min(min(max(x1, x2), max(y1, y2)), max(z1, z2));
	

	surf.t = tMin;
	if (tMin < tMax && tMax > 0 && surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		float EPS = 0.001;
		if (abs(surf.hitPoint.x - t_min.x) < EPS)
			surf.normal = float3(-1, 0, 0);
		else if (abs(surf.hitPoint.x - t_min.x) < EPS)
			surf.normal = float3(1, 0, 0);
		else if (abs(surf.hitPoint.y - t_min.y) < EPS)
			surf.normal = float3(0, -1, 0);
		else if (abs(surf.hitPoint.y - t_max.y) < EPS)
			surf.normal = float3(0, 1, 0);
		else if (abs(surf.hitPoint.z - t_min.z) < EPS)
			surf.normal = float3(0, 0, -1);
		else if (abs(surf.hitPoint.z - t_max.z) < EPS)
			surf.normal = float3(0, 0, 1);

		//surf.normal = normalize(surf.hitPoint - t_min);
		surf.m_ptr = m_ptr;
		surf.v = 1.0f;
		surf.u = 1.0f;
		return true;
	}
	return false;
}

bool Triangle::Intersect(const Ray &ray, float tmin, float tmax, SurfHit &surf) const
{
	/*float3 normal = cross((b - a), (c - a));
	if (dot(ray.d, normal) < 0) return false;
	float3 t_a, t_b, t_v;
	t_a = a - ray.o;
	t_b = b - ray.o;
	t_v = cross(t_a, t_b);
	double ip1 = dot(ray.d, t_v);
	t_a = b - ray.o;
	t_b = c - ray.o;
	t_v = cross(t_a, t_b);
	double ip2 = dot(ray.d, t_v);
	t_a = c - ray.o;
	t_b = a - ray.o;
	t_v = cross(a, b);
	double ip3 = dot(ray.d, t_v);

	if (((ip1 < 0) && (ip2 < 0) && (ip3 < 0)) || ((ip1 > 0) && (ip2 > 0) && (ip3 > 0)))
	{
		float d = dot(normal, ray.d);
		if (fabs(d) < 0) return false;
		float distanse = 0 - normal.x * ray.o.x - normal.y * ray.o.y - normal.z * ray.o.z;
		float ips = (-dot(normal, ray.o) - distanse) / d;
		surf.t = ips;
		if (ips >= 0) {
			surf.hit = true;
			surf.hitPoint = ray.o + surf.t * ray.d;
			surf.normal = normal;
			surf.m_ptr = m_ptr;
			return true;
		}
	}
	return false;*/
	float3 E1 = b - a;
	float3 E2 = c - a;
	float3 T = ray.o - a;
	float3 D = ray.d;
	float3 P  = cross(D, E2);
	float3 Q = cross(T, E1);
	float det = dot(E1, P);

	if (det < tmin && det > tmax) {
		return false;
	}

	float invDet = 1 / det;
	float u = dot(T, P) * invDet;

	if (u < 0 || u > 1) {
		return false;
	}

	float v = dot(ray.d, Q) * invDet;

	if (v < 0 || u + v > 1) {
		return false;
	}

	surf.t = dot(E2, Q) * invDet;
	if (surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = float3(surf.t, u, v);
		surf.normal = cross((b-a),(c-a));
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}


bool Disk::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float denom = dot(n, ray.d);
	float t;
	if (denom > tmin && denom < tmax) {
		float3 p0l0 = o - ray.o;
		t = dot(p0l0, n) / denom;
		if (t >= 0) {
			float3 p = ray.o + ray.d * t;
			float3 v = p - o;
			float d2 = dot(v, v);
			if (sqrtf(d2) <= r) {
				surf.t = t;
				surf.hit = true;
				surf.hitPoint = ray.o + t * ray.d;
				surf.normal = normalize(surf.hitPoint);
				surf.m_ptr = m_ptr;
				surf.v = 1.0f;
				surf.u = 1.0f;
				return true;
			}
		}
	}

	return false;

}