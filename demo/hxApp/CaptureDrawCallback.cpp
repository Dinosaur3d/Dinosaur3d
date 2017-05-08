#include "CaptureDrawCallback.h"
#include <osgDB/WriteFile>


CaptureDrawCallback::CaptureDrawCallback(void)
{
	_image = 0;
}


CaptureDrawCallback::~CaptureDrawCallback(void)
{
}

void CaptureDrawCallback::operator()( const osg::Camera& camera ) const
{
	if (_savepath.size() <= 0)
	{
		return;
	}
	osg::Image* scr_image_rgb = new osg::Image();
	unsigned int width, height = 0;
	//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
	width = camera.getViewport()->width();

	height = camera.getViewport()->height();

    //scr_image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
    //scr_image_rgb->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);
    scr_image_rgb->readPixels(0,0,width,height,GL_DEPTH_COMPONENT,GL_FLOAT);
	bool res = osgDB::writeImageFile(*scr_image_rgb,_savepath.c_str());
	const_cast<CaptureDrawCallback*>(this)->_savepath = "";
	//const_cast<osg::Camera*>(&camera)->setPostDrawCallback(0);
}
