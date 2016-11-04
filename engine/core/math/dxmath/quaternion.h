#pragma once

#ifndef INLINE
#define INLINE __forceinline
#endif

#include "pch.h"

#include "scalar.h"
#include "vector3.h"
#include "mat44.h"

namespace engine
{
	class Quaternion
	{
	public:
		INLINE Quaternion() { q = DirectX::XMQuaternionIdentity(); }
		INLINE Quaternion(const DirectX::XMVECTOR& q_) { q = q_; }
		INLINE Quaternion(const Scalar& roll, const Scalar& pitch, const Scalar& yaw) { q = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
		INLINE Quaternion(const Vector3& axis, const Scalar& angle) { q = DirectX::XMQuaternionRotationAxis(axis, angle); }
		INLINE Quaternion(const Vector3& rotation_radians) { q = DirectX::XMQuaternionRotationRollPitchYaw(rotation_radians.GetX(), rotation_radians.GetY(), rotation_radians.GetZ()); }
		INLINE Quaternion(const Mat44& matrix) { q = DirectX::XMQuaternionRotationMatrix(matrix); }
		INLINE Quaternion(const physx::PxQuat& px_quat) { q = DirectX::XMVectorSet(px_quat.x, px_quat.y, px_quat.z, px_quat.w); }

		INLINE operator DirectX::XMVECTOR()	const { return q; }

		INLINE Scalar GetX() const { return Scalar(DirectX::XMVectorGetX(q)); }
		INLINE Scalar GetY() const { return Scalar(DirectX::XMVectorGetY(q)); }
		INLINE Scalar GetZ() const { return Scalar(DirectX::XMVectorGetZ(q)); }
		INLINE Scalar GetW() const { return Scalar(DirectX::XMVectorGetW(q)); }
		INLINE void SetX(const Scalar& s) { DirectX::XMVectorSetX(q, s.Get()); }
		INLINE void SetY(const Scalar& s) { DirectX::XMVectorSetY(q, s.Get()); }
		INLINE void SetZ(const Scalar& s) { DirectX::XMVectorSetZ(q, s.Get()); }
		INLINE void SetW(const Scalar& s) { DirectX::XMVectorSetW(q, s.Get()); }

		INLINE std::string ToString() const { return std::string(GetX()) + "," + std::string(GetY()) + "," + std::string(GetZ()) + "," + std::string(GetW()); }
		INLINE operator std::string() const { return ToString(); }

		INLINE Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(q)); }
		INLINE Quaternion operator- (void) const { return Quaternion(DirectX::XMVectorNegate(q)); }

		INLINE Quaternion operator* (Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(q, rhs.q)); }
		INLINE Vector3 operator* (Vector3 rhs) const { return Vector3(DirectX::XMVector3Rotate(rhs, q)); }

		INLINE Quaternion& operator= (Quaternion rhs) { q = rhs.q; return *this; }
		INLINE Quaternion& operator*= (Quaternion rhs) { *this = *this * rhs; return *this; }

		INLINE bool operator== (const Quaternion& rhs) const { return (GetX() == rhs.GetX() && GetY() == rhs.GetY() && GetZ() == rhs.GetZ() && GetW() == rhs.GetW()); }
		INLINE bool operator!= (const Quaternion& rhs) const { return !(GetX() == rhs.GetX() && GetY() == rhs.GetY() && GetZ() == rhs.GetZ() && GetW() == rhs.GetW()); }

		INLINE physx::PxQuat ToPxQuat() const
		{
			physx::PxQuat pxq;

			pxq.x = GetX();
			pxq.y = GetY();
			pxq.z = GetZ();
			pxq.w = GetW();

			return pxq;
		}

		INLINE Vector3 ToEuler() const {
			double sqw;
			double sqx;
			double sqy;
			double sqz;
			
			float rotxrad;
			float rotyrad;
			float rotzrad;
			
			sqw = DirectX::XMVectorGetW(q) * DirectX::XMVectorGetW(q);
			sqx = DirectX::XMVectorGetX(q) * DirectX::XMVectorGetX(q);
			sqy = DirectX::XMVectorGetY(q) * DirectX::XMVectorGetY(q);
			sqz = DirectX::XMVectorGetZ(q) * DirectX::XMVectorGetZ(q);
			
			rotxrad = (float)atan2l(2.0 * (DirectX::XMVectorGetY(q) * DirectX::XMVectorGetZ(q) + DirectX::XMVectorGetX(q) * DirectX::XMVectorGetW(q)), (-sqx - sqy + sqz + sqw));
			rotyrad = (float)asinl(-2.0 * (DirectX::XMVectorGetX(q) * DirectX::XMVectorGetZ(q) - DirectX::XMVectorGetY(q) * DirectX::XMVectorGetW(q)));
			rotzrad = (float)atan2l(2.0 * (DirectX::XMVectorGetX(q) * DirectX::XMVectorGetY(q) + DirectX::XMVectorGetZ(q) * DirectX::XMVectorGetW(q)), (sqx - sqy - sqz + sqw));
			
			return Vector3(rotxrad, rotyrad, rotzrad);
		};

	private:
		DirectX::XMVECTOR q;
	};
}