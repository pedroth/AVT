#pragma once

#include <vector>
#include <cassert>
#include <glm.hpp>

template <class Mat>
class MatrixStack {
	std::vector<Mat> _mats;
	Mat _currMat;
public:
	MatrixStack();
	MatrixStack(Mat initMat);
	void loadMat(Mat mat);
	void multiplyMat(Mat mat);
	void push();
	void pop();
	Mat current();
	unsigned int stackSize();
};

extern MatrixStack<glm::mat4> ModelMatrixStack;


//////////////////////////////////////////////////////////////////



template<class Mat>
MatrixStack<Mat>::MatrixStack()
: _mats(), _currMat()
{}

template<class Mat>
MatrixStack<Mat>::MatrixStack(Mat initMat)
: _mats(), _currMat(initMat)
{}

template<class Mat>
void MatrixStack<Mat>::loadMat(Mat mat)
{
	_currMat = mat;
}

template<class Mat>
void MatrixStack<Mat>::multiplyMat(Mat mat)
{
	_currMat = _currMat * mat;
}

template<class Mat>
void MatrixStack<Mat>::push()
{
	_mats.push_back(_currMat);
}

template<class Mat>
void MatrixStack<Mat>::pop()
{
	assert(_mats.size() >= 1);
	_currMat = _mats.back();
	_mats.pop_back();
}

template<class Mat>
Mat MatrixStack<Mat>::current()
{
	return _currMat;
}

template<class Mat>
unsigned int MatrixStack<Mat>::stackSize()
{
	return _mats.size();
}