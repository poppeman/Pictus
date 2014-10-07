#ifndef GEOM_H
#define GEOM_H

namespace Geom {
	template <typename T>
	class Size {
	public:
		T							Width, Height;

		Size<T> Flipped() const { return Size<T>(Height, Width); }

		Size<T> operator-() const { return Size<T>(-Width, -Height); }

		template <typename U> Size<T>& operator=(const Size<U>& rhs) { Width = rhs.Width; Height = rhs.Height; return *this; }

		Size<T> operator+(const Size<T>& rhs) const { return Size<T>(Width + rhs.Width, Height + rhs.Height); }
		Size<T> operator-(const Size<T>& rhs) const { return Size<T>(Width - rhs.Width, Height - rhs.Height); }
		Size<T> operator/(const Size<T>& rhs) const { return Size<T>(Width / rhs.Width, Height / rhs.Height); }
		template <typename U> Size<T> operator/(const U& rhs) const { return (*this) / Size<T>(rhs, rhs); }

		template <typename U> Size<T> operator*(const U rhs) const { return Size<T>(Width * rhs, Height * rhs); }
		Size<float> operator*(const float rhs) const { return Size<float>(Width * rhs, Height * rhs); }
		Size<T> operator*(const Size<T>& rhs) const { return Size<T>(Width * rhs.Width, Height * rhs.Height); }

		Size<T>& operator +=(const Size<T>& rhs) { (*this) = (*this) + rhs; return (*this); }
		Size<T>& operator -=(const Size<T>& rhs) { (*this) = (*this) - rhs; return (*this); }

		bool operator==(const Size<T>& b) const { return (Width == b.Width) && (Height == b.Height); }
		bool operator!=(const Size<T>& b) const { return !(*this == b); }

		Size<T> operator-() { return Size<T>(-Width, -Height); }

		operator Size<float>() const { return Size<float>(static_cast<float>(Width), static_cast<float>(Height)); }
		template <typename U> Size<U> StaticCast() const { return Size<U>(static_cast<U>(Width), static_cast<U>(Height)); }

		bool AtLeastExclusive(const Geom::Size<T>& border) const {
			return (Width > border.Width && Height > border.Height);
		}
		bool AtLeastInclusive(const Geom::Size<T>& border) const {
			return (Width >= border.Width && Height >= border.Height);
		}

		Size(T width, T height):Width(width), Height(height) {}
		Size():Width(0), Height(0) {}
	};

	template <typename T> Size<T> operator*(const T lhs, const Size<T> rhs) { return rhs * lhs; }
	template <typename T> Size<float> operator /(float lhs, const Size<T>& rhs) { return Size<float>(lhs / rhs.Width, lhs / rhs.Height); }

	template <typename T>
	class Point {
	public:
		T X, Y;

		Point<T> Flipped() const { return Point<T>(Y, X); }

		Point<T> operator/(const Size<T>& rhs) const { return Point<T>(X / rhs.Width, Y / rhs.Height); }
		template <typename U> Point<T> operator/(const U& rhs) const { return (*this) / Size<T>(rhs, rhs); }

		template <typename U> Point<T> operator*(const U rhs) const { return Point<T>(X * rhs, Y * rhs); }
		Point<T> operator *(const Size<T>& rhs) const { return Point<T>(X * rhs.Width, Y * rhs.Height); }
		Point<float> operator *(const float rhs) const { return Point<float>(X * rhs, Y * rhs); }


		Point<T> operator +(const Point<T>& rhs) const { return Point<T>(X + rhs.X, Y + rhs.Y); }
		Point<T> operator +(const Size<T>& rhs) const { return Point(X + rhs.Width, Y + rhs.Height); }

		Size<T> operator -(const Point<T>& rhs) const { return Size<T>(X - rhs.X, Y - rhs.Y); }
		Point<T> operator -(const Size<T>& rhs) const { return Point<T>(X - rhs.Width, Y - rhs.Height); }

		Point<T>& operator +=(const Point<T>& rhs) { X += rhs.X; Y += rhs.Y; return (*this); }
		Point<T>& operator +=(const Size<T>& rhs) { (*this) = (*this) + rhs; return (*this); }
		Point<T>& operator -=(const Size<T>& rhs) { *this = *this - rhs; return (*this); }

		bool operator !=(const Point<T>& rhs) const { return !(*this == rhs); }

		Point<T> operator -() const { return Point<T>(-X, -Y); }


		operator Point<float>() const { return Point<float>(static_cast<float>(X), static_cast<float>(Y)); }

		template <typename U> Point<U> StaticCast() const { return Point<U>(static_cast<U>(X), static_cast<U>(Y)); }

		bool AtLeastExclusive(const Geom::Point<T>& border) const {
			return (X > border.X && Y > border.Y);
		}
		bool AtLeastInclusive(const Geom::Point<T>& border) const {
			return (X >= border.X && Y >= border.Y);
		}

		bool AtMostExclusive(const Geom::Point<T>& border) const {
			return (X < border.X && Y < border.Y);
		}

		bool AtMostInclusive(const Geom::Point<T>& border) const {
			return (X <= border.X && Y <= border.Y);
		}

		Point(T x, T y)
			:X(x),
			 Y(y)
		{}

		Point()
			:X(0),
			 Y(0)
		{}
	};

	template <typename T>
	class Rect {
	public:
		const T& Width() const { return m_size.Width; }
		const T& Height() const { return m_size.Height; }
		const T& Left() const { return m_topLeft.X; }

		void Left(T X) { m_topLeft.X = X; }
		const T& Top() const { return m_topLeft.Y; }
		void Top(T Y) { m_topLeft.Y = Y; }

		const T Right() const { return m_topLeft.X + m_size.Width; }
		void Right(const int nr) { m_size.Width = nr - m_topLeft.X; }
		const T Bottom() const { return m_topLeft.Y + m_size.Height; }
		void Bottom(const int nb) { m_size.Height = nb - m_topLeft.Y; }

		Size<T> Dimensions() const { return m_size; }
		void Dimensions(const Size<T>& sz) { m_size = sz; }

		const Point<T>& TopLeft() const { return m_topLeft; }
		void TopLeft(const Point<T>& newTl) { m_topLeft = newTl; }

		const Point<T> TopRight() const { return Geom::Point<T>(Right(), Top()); }
		const Point<T> BottomLeft() const { return Geom::Point<T>(Left(), Bottom()); }
		const Point<T> BottomRight() const { return Geom::Point<T>(Right(), Bottom()); }
		void BottomRight(const Point<T>& newBr) { Bottom(newBr.Y); Right(newBr.X); }

		bool Contains(const Point<T>& pt) const {
			if (pt.X >= Left() && pt.X < Right() && pt.Y >= Top() && pt.Y < Bottom())
				return true;

			return false;
		}

		bool Contains(const Rect<T>& pt) const {
			return (Contains(pt.TopLeft()) && Contains(pt.BottomRight() - Size<T>{1, 1}));
		}

		Rect<T> Crop(const Rect<T>& toCrop) const {
			Rect<T> r(toCrop);
			r.TopLeft(Maximum(toCrop.TopLeft(), TopLeft()));
			r.BottomRight(Minimum(toCrop.BottomRight(), BottomRight()));
			return r;
		}

		template <typename U>
		Rect<U> StaticCast() const { return Rect<U>(TopLeft().StaticCast<U>(), Dimensions().StaticCast<U>()); }

		Rect(const Point<T>& topLeft, const Size<T>& rectSize)
			:m_topLeft(topLeft), m_size(rectSize)
		{}

		Rect(const Point<T>& p1, const Point<T>& p2)
			:m_topLeft(Point<T>(Util::Min(p1.X, p2.X), Util::Min(p1.Y, p2.Y))), m_size(Size<T>(abs(p1.X-p2.X), abs(p1.Y-p2.Y)))
		{}

		Rect() {}

	private:
		Size<T> m_size;
		Point<T> m_topLeft;
	};

	typedef Size<int> SizeInt;
	typedef Point<int> PointInt;
	typedef Rect<int> RectInt;

	typedef Size<float> SizeFloat;
	typedef Point<float> PointFloat;
	typedef Rect<float> RectFloat;

	template <typename T> inline T Area(const Geom::Size<T>& size) { return size.Width * size.Height; }
	template <typename T> inline bool operator ==(const Point<T>& a, const Point<T>& b) { return (a.X == b.X && a.Y == b.Y); }
	template <typename T> inline bool operator ==(const Rect<T>& a, const Rect<T>& b) { return (a.Dimensions() == b.Dimensions()) && (a.TopLeft() == b.TopLeft()); }

	template <typename T> inline bool AreaNonZero(const Geom::Size<T>& size) { return  size.Width > 0 && size.Height > 0; }

	inline SizeInt RoundCast(const Size<float>& rhs) { return (Size<float>(0.5f, 0.5f) + rhs).StaticCast<int>(); }
	inline PointInt RoundCast(const Point<float>& rhs) { return (Point<float>(0.5f, 0.5f) + rhs).StaticCast<int>(); }
}

namespace Geom {
	template<typename T>
	Geom::Point<T> Maximum(const Geom::Point<T>& a, const Geom::Point<T>& b) {
		return Geom::Point<T>(Util::Max(a.X, b.X), Util::Max(a.Y, b.Y));
	}

	template<typename T>
	Geom::Point<T> Minimum(const Geom::Point<T>& a, const Geom::Point<T>& b) {
		return Geom::Point<T>(Util::Min(a.X, b.X), Util::Min(a.Y, b.Y));
	}


	template<typename T>
	Geom::Size<T> Maximum(const Geom::Size<T>& a, const Geom::Size<T>& b) {
		return Geom::Size<T>(Util::Max(a.Width, b.Width), Util::Max(a.Height, b.Height));
	}

	template<typename T>
	Geom::Size<T> Minimum(const Geom::Size<T>& a, const Geom::Size<T>& b) {
		return Geom::Size<T>(Util::Min(a.Width, b.Width), Util::Min(a.Height, b.Height));
	}

	template<typename T>
	Geom::Size<T> Minimum(const Geom::Size<T>& a, const Geom::Size<T>& b, const Geom::Size<T>& c) {
		return Minimum(a, Minimum(b, c));
	}

	template<typename T>
	Geom::Size<T> Constrain(const Geom::Size<T>& lower, const Geom::Size<T>& x, const Geom::Size<T>& upper) {
		return Minimum(Maximum(lower, x), upper);
	}

	template<typename T>
	Geom::Point<T> Constrain(const Geom::Point<T>& lower, const Geom::Point<T>& x, const Geom::Point<T>& upper) {
		return Minimum(Maximum(lower, x), upper);
	}

	template<typename T>
	bool AtMost(const Geom::Size<T>& a, const Geom::Size<T>& border) {
		return (a.Width < border.Width && a.Height < border.Height);
	}

	template<typename T>
	bool IsPositive(const T& a) {
		return a.AtLeastExclusive(T(0, 0));
	}

	template<typename T>
	bool IsZeroOrPositive(const T& a) {
		return a.AtLeastInclusive(T(0, 0));
	}
}

template <typename T>
std::wstring ToWString(const Geom::Point<T>& pt) {
	return TX("(") + ToWString(pt.X) + TX(", ") + ToWString(pt.Y) + TX(")");
}

template <typename T>
inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Geom::Point<T>& v) {
	in << "(" << (T)v.X << ", " << (T)v.Y << ")";
	return in;
}

template <typename _c, typename _t>
inline std::basic_ostream<_c>& operator<<(std::basic_ostream<_c>& in, const Geom::Size<_t>& v) {
	in << "(" << (_t)v.Width << "x" << (_t)v.Height << ")";
	return in;
}

inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Geom::RectInt& v) {
	in << "{" << v.TopLeft() << ", " << v.Dimensions() << "}";
	return in;
}

#endif
