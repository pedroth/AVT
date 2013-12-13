#pragma once

#include <vector>
#include <cassert>
#include <glm.hpp>

template <class Mat>
class MatrixStack {
	std::vector<Mat> _mats;
	Mat _currMat;
public:
	//Create a new MatrixStack.
	//Current matrix stack is default.
	MatrixStack();
	//Create a new MatrixStack.
	//Current matrix is given.
	MatrixStack(Mat initMat);
	//Set current matrix.
	void loadMat(Mat mat);
	//Multiply mat by current matrix.
	//curr = mat * curr;
	void multiplyMat(Mat mat);
	//Push current matrix onto the stack.
	void push();
	//Pop stack top onto current matrix.
	void pop();
	//Return copy of current matrix.
	Mat current();
	//Return matrixstack size.
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