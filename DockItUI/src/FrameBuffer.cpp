#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer() {
}
FrameBuffer::~FrameBuffer() {
}


void FrameBuffer::createFrameBuffer(int width, int height) {
	//Frame buffers allow us to render to multiple viewports such as the left and right eye and companion window.
	//Basically we are rendering to a texture instead of the screen.
	//This allows us to render to multiple viewports without having to render the same scene multiple times.
	//This is a very useful technique for VR. - george

	//Generate a frame buffer and bind it.
	glGenFramebuffers(1, &this->m_nRenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_nRenderFramebufferId);

	//Generate a render buffer, bind it and specify details about the buffer.
	glGenRenderbuffers(1, &this->m_nDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_nDepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	//Unbind the render buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Attach the render buffer to the frame buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_nDepthBufferId);

	//Generate a texture, bind it and specify details about the texture.
	glGenTextures(1, &this->m_nRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, this->m_nRenderTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//Set texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach the texture to the frame buffer.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_nRenderTextureId, 0);

	//Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer creation successful." << std::endl;
	}
	else {
		std::cout << "Framebuffer creation FAILED." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
