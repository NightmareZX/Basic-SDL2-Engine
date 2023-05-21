#pragma once
#include <iostream>
#include <string>

template<class T>
/// <summary>
/// A simple class for handling matrices using heap allocated 2D Arrays.
/// The class does not index check or deallocate the template type elements
/// </summary>
/// <typeparam name="T"></typeparam>
class Matrix2D
{
private:
	//despite this class being private inside Matrix2D, outside objects can still access its [] operator to change and use its data
	//but still disallow the outside usage of the _proxyRow class
	class _proxyRow
	{
	private:
		friend class Matrix2D;
		T* mRow;
		size_t mWidth;
		_proxyRow(T* rowData, size_t width) : mRow(rowData), mWidth(width) {}
	public:
		T& operator[](size_t index)
		{
			if (index >= mWidth)
			{
				throw std::out_of_range("Column Index out of range! Size: " + std::to_string(mWidth) + " Index: " + std::to_string(index));
			}
			return mRow[index];
		}
	};
	T** mMatrix;

	size_t mWidth, mHeight;
public:
	//regular constructor
	Matrix2D(size_t width, size_t height, T defaultValue)
	{
		mMatrix = new T * [height];
		for (size_t row = 0; row < height; row++)
		{
			mMatrix[row] = new T[width];
			for (size_t column = 0; column < width; column++)
			{
				mMatrix[row][column] = defaultValue;
			}
		}
		mWidth = width;
		mHeight = height;
	}
	//copy contructor
	Matrix2D(const Matrix2D& other)
	{
		mMatrix = new T * [other.mHeight];
		for (size_t row = 0; row < other.mHeight; row++)
		{
			mMatrix[row] = new T[other.mWidth];
			for (size_t column = 0; column < other.mWidth; column++)
			{
				mMatrix[row][column] = mMatrix[row][column] = other.mMatrix[row][column];
			}
		}
		mWidth = other.mWidth;
		mHeight = other.mHeight;
	}
	//default constructor
	Matrix2D()
	{
		mMatrix = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
	~Matrix2D()
	{
		for (size_t row = 0; row < mHeight; row++)
		{
			delete[] mMatrix[row];
		}
		delete[] mMatrix;
	}
	_proxyRow operator[] (size_t index)
	{
		if (index >= mHeight)
		{
			throw std::out_of_range("Row Index out of range! Size: " + std::to_string(mWidth) + " Index: " + std::to_string(index));
		}
		return _proxyRow(mMatrix[index], mWidth);
	}
	Matrix2D& operator=(const Matrix2D& other)
	{
		if (this == &other)//if its the same object, dont bother copying
		{
			return *this;
		}
		for (size_t row = 0; row < mHeight; row++)
		{
			delete[] mMatrix[row];
		}
		delete[] mMatrix;
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mMatrix = new T * [mHeight];
		for (size_t row = 0; row < mHeight; row++)
		{
			mMatrix[row] = new T[mWidth];
			//std::copy(other.mMatrix[row], other.mMatrix[row] + mWidth, mMatrix[row]);
			for (size_t column = 0; column < mWidth; column++)
			{
				mMatrix[row][column] = other.mMatrix[row][column];
			}
		}
		return *this;
	}
	inline size_t GetWidth() { return mWidth; }
	inline size_t GetHeight() { return mHeight; }
};