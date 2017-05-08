#pragma once
#include <osg/Camera>
#include <string>

class CaptureDrawCallback : public osg::Camera::DrawCallback
{
public:
	CaptureDrawCallback(void);
	~CaptureDrawCallback(void);

	virtual void operator()(const osg::Camera& camera) const;
	bool _active;
	std::string _savepath;
	osg::ref_ptr<osg::Image> _image;
};

