#pragma once
#include <GL/glew.h>

class FrameBuffer
{
public:

		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;

		FrameBuffer();
		~FrameBuffer();
		void createFrameBuffer(int width, int height);
};

