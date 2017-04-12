
--void glShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length)
--glShaderSource(<int>shader, <string>string)


--void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders)
--local <int> shaders = glGetAttachedShaders(<int>program, <int>maxCount)


--void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data)
--local <string> data = glReadPixels(<int>x, <int>y, <int>width, <int>height, <int>format, <int>type)


--void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
--glColorMask(<bool>red, <bool>green, <bool>blue, <bool>alpha)


--void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
--glVertexAttribPointer(<int>index, <int>size, <int>type, <bool>normalized, <int>stride, <string>pointer)


--void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
--glCompressedTexSubImage2D(<int>target, <int>level, <int>xoffset, <int>yoffset, <int>width, <int>height, <int>format, <int>imageSize, <string>data)


--void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
--glBlendEquationSeparate(<int>modeRGB, <int>modeAlpha)


--void glDepthMask(GLboolean flag)
--glDepthMask(<bool>flag)


--void glClear(GLbitfield mask)
--glClear(<int>mask)


--void glSampleCoverage(GLclampf value, GLboolean invert)
--glSampleCoverage(<float>value, <bool>invert)


--void glFlush()
--glFlush()


--void glGetShaderPrecisionFormat(GLenum shaderType, GLenum precisionType, GLint * range, GLint * precision)
--local <int>range, <int>precision = glGetShaderPrecisionFormat(<int>shaderType, <int>precisionType)


--void glClearStencil(GLint s)
--glClearStencil(<int>s)


--void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
--glDeleteFramebuffers(<int>n, {[int]}framebuffers)


--void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
--glBlendColor(<float>red, <float>green, <float>blue, <float>alpha)


--GLenum glCheckFramebufferStatus(GLenum target)
--local <int>_ll_ret = glCheckFramebufferStatus(<int>target)


--void glDepthFunc(GLenum func)
--glDepthFunc(<int>func)


--void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params)
--local <int>params = glGetFramebufferAttachmentParameteriv(<int>target, <int>attachment, <int>pname)


--GLuint glCreateProgram()
--local <int>_ll_ret = glCreateProgram()


--void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
--glStencilOp(<int>sfail, <int>dpfail, <int>dppass)


--void glGenTextures(GLsizei n, GLuint * textures)
--local {[int]}textures = glGenTextures(<int>n)


--void glGenerateMipmap(GLenum target)
--glGenerateMipmap(<int>target)


--GLboolean glIsEnabled(GLenum cap)
--local <bool>_ll_ret = glIsEnabled(<int>cap)


--void glDrawArrays(GLenum mode, GLint first, GLsizei count)
--glDrawArrays(<int>mode, <int>first, <int>count)


--GLboolean glIsTexture(GLuint texture)
--local <bool>_ll_ret = glIsTexture(<int>texture)


--void glDeleteShader(GLuint shader)
--glDeleteShader(<int>shader)


--void glTexParameterf(GLenum target, GLenum pname, GLfloat param)
--glTexParameterf(<int>target, <int>pname, <float>param)


--void glTexParameteri(GLenum target, GLenum pname, GLint param)
--glTexParameteri(<int>target, <int>pname, <int>param)


--void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params)
--glTexParameterfv(<int>target, <int>pname, {[float]}params)


--void glTexParameteriv(GLenum target, GLenum pname, const GLint * params)
--glTexParameteriv(<int>target, <int>pname, {[int]}params)


--GLuint glCreateShader(GLenum shaderType)
--local <int>_ll_ret = glCreateShader(<int>shaderType)


--void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
--glScissor(<int>x, <int>y, <int>width, <int>height)


--void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data)
--glTexImage2D(<int>target, <int>level, <int>internalformat, <int>width, <int>height, <int>border, <int>format, <int>type, <string>data)


--void glActiveTexture(GLenum texture)
--glActiveTexture(<int>texture)


--GLboolean glIsRenderbuffer(GLuint renderbuffer)
--local <bool>_ll_ret = glIsRenderbuffer(<int>renderbuffer)


--void glStencilMaskSeparate(GLenum face, GLuint mask)
--glStencilMaskSeparate(<int>face, <int>mask)


--void glGenBuffers(GLsizei n, GLuint * buffers)
--local {[int]}buffers = glGenBuffers(<int>n)


--void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
--glClearColor(<float>red, <float>green, <float>blue, <float>alpha)


--void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data)
--glCompressedTexImage2D(<int>target, <int>level, <int>internalformat, <int>width, <int>height, <int>border, <int>imageSize, <string>data)


--void glDeleteProgram(GLuint program)
--glDeleteProgram(<int>program)


--void glDeleteBuffers(GLsizei n, const GLuint * buffers)
--glDeleteBuffers(<int>n, {[int]}buffers)


--void glEnable(GLenum cap)
--glEnable(<int>cap)


--void glDisable(GLenum cap)
--glDisable(<int>cap)


--GLint glGetAttribLocation(GLuint program, const GLchar * name)
--local <int>_ll_ret = glGetAttribLocation(<int>program, <string>name)


--void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
--glStencilFuncSeparate(<int>face, <int>func, <int>ref, <int>mask)


--void glFrontFace(GLenum mode)
--glFrontFace(<int>mode)


--void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data)
--glBufferSubData(<int>target, <int>offset, <int>size, <string>data)


--void glGenFramebuffers(GLsizei n, GLuint * framebuffers)
--local {[int]}framebuffers = glGenFramebuffers(<int>n)


--void glEnableVertexAttribArray(GLuint index)
--glEnableVertexAttribArray(<int>index)


--void glDisableVertexAttribArray(GLuint index)
--glDisableVertexAttribArray(<int>index)


--void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
--glFramebufferRenderbuffer(<int>target, <int>attachment, <int>renderbuffertarget, <int>renderbuffer)


--void glShaderBinary(GLsizei n, const GLuint * shaders, GLenum binaryformat, const void * binary, GLsizei length)
--glShaderBinary(<int>n, {[int]}shaders, <int>binaryformat, <string>binary, <int>length)


--void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)
--local <int>length, <int>size, <int>type, <string>name = glGetActiveUniform(<int>program, <int>index, <int>bufSize)


--void glCullFace(GLenum mode)
--glCullFace(<int>mode)


--void glReleaseShaderCompiler()
--glReleaseShaderCompiler()


--void glStencilFunc(GLenum func, GLint ref, GLuint mask)
--glStencilFunc(<int>func, <int>ref, <int>mask)


--void glGetShaderiv(GLuint shader, GLenum pname, GLint * params)
--local <int>params = glGetShaderiv(<int>shader, <int>pname)


--void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
--glBufferData(<int>target, <int>size, <string>data, <int>usage)


--void glUseProgram(GLuint program)
--glUseProgram(<int>program)


--void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
--glStencilOpSeparate(<int>face, <int>sfail, <int>dpfail, <int>dppass)


--void glGetUniformfv(GLuint program, GLint location, GLfloat * params)
--local <float>params = glGetUniformfv(<int>program, <int>location)


--void glGetUniformiv(GLuint program, GLint location, GLint * params)
--local <int>params = glGetUniformiv(<int>program, <int>location)


--void glBindFramebuffer(GLenum target, GLuint framebuffer)
--glBindFramebuffer(<int>target, <int>framebuffer)


--void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
--glFramebufferTexture2D(<int>target, <int>attachment, <int>textarget, <int>texture, <int>level)


--void glLinkProgram(GLuint program)
--glLinkProgram(<int>program)


--void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers)
--local {[int]}renderbuffers = glGenRenderbuffers(<int>n)


--void glGetBooleanv(GLenum pname, GLboolean * params)
--local <bool>params = glGetBooleanv(<int>pname)


--void glGetFloatv(GLenum pname, GLfloat * params)
--local <float>params = glGetFloatv(<int>pname)


--void glGetIntegerv(GLenum pname, GLint * params)
--local <int>params = glGetIntegerv(<int>pname)


--void glUniform1f(GLint location, GLfloat v0)
--glUniform1f(<int>location, <float>v0)


--void glUniform2f(GLint location, GLfloat v0, GLfloat v1)
--glUniform2f(<int>location, <float>v0, <float>v1)


--void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
--glUniform3f(<int>location, <float>v0, <float>v1, <float>v2)


--void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
--glUniform4f(<int>location, <float>v0, <float>v1, <float>v2, <float>v3)


--void glUniform1i(GLint location, GLint v0)
--glUniform1i(<int>location, <int>v0)


--void glUniform2i(GLint location, GLint v0, GLint v1)
--glUniform2i(<int>location, <int>v0, <int>v1)


--void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
--glUniform3i(<int>location, <int>v0, <int>v1, <int>v2)


--void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
--glUniform4i(<int>location, <int>v0, <int>v1, <int>v2, <int>v3)


--void glUniform1fv(GLint location, GLsizei count, const GLfloat * value)
--glUniform1fv(<int>location, <int>count, {[float]}value)


--void glUniform2fv(GLint location, GLsizei count, const GLfloat * value)
--glUniform2fv(<int>location, <int>count, {[float]}value)


--void glUniform3fv(GLint location, GLsizei count, const GLfloat * value)
--glUniform3fv(<int>location, <int>count, {[float]}value)


--void glUniform4fv(GLint location, GLsizei count, const GLfloat * value)
--glUniform4fv(<int>location, <int>count, {[float]}value)


--void glUniform1iv(GLint location, GLsizei count, const GLint * value)
--glUniform1iv(<int>location, <int>count, {[int]}value)


--void glUniform2iv(GLint location, GLsizei count, const GLint * value)
--glUniform2iv(<int>location, <int>count, {[int]}value)


--void glUniform3iv(GLint location, GLsizei count, const GLint * value)
--glUniform3iv(<int>location, <int>count, {[int]}value)


--void glUniform4iv(GLint location, GLsizei count, const GLint * value)
--glUniform4iv(<int>location, <int>count, {[int]}value)


--void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
--glUniformMatrix2fv(<int>location, <int>count, <bool>transpose, {[float]}value)


--void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
--glUniformMatrix3fv(<int>location, <int>count, <bool>transpose, {[float]}value)


--void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
--glUniformMatrix4fv(<int>location, <int>count, <bool>transpose, {[float]}value)


--void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
--glCopyTexSubImage2D(<int>target, <int>level, <int>xoffset, <int>yoffset, <int>x, <int>y, <int>width, <int>height)


--void glBindRenderbuffer(GLenum target, GLuint renderbuffer)
--glBindRenderbuffer(<int>target, <int>renderbuffer)


--void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices)
--glDrawElements(<int>mode, <int>count, <int>type, <string>indices)


--const GLubyte* glGetString(GLenum name)
--local <string>_ll_ret = glGetString(<int>name)


--void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
--glViewport(<int>x, <int>y, <int>width, <int>height)


--void glBlendEquation(GLenum mode)
--glBlendEquation(<int>mode)


--void glBindBuffer(GLenum target, GLuint buffer)
--glBindBuffer(<int>target, <int>buffer)


--GLenum glGetError()
--local <int>_ll_ret = glGetError()


--void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params)
--local <float>params = glGetVertexAttribfv(<int>index, <int>pname)


--void glGetVertexAttribiv(GLuint index, GLenum pname, GLint * params)
--local <int>params = glGetVertexAttribiv(<int>index, <int>pname)


--void glCompileShader(GLuint shader)
--glCompileShader(<int>shader)


--void glBindTexture(GLenum target, GLuint texture)
--glBindTexture(<int>target, <int>texture)


--void glFinish()
--glFinish()


--void glDepthRangef(GLclampf nearVal, GLclampf farVal)
--glDepthRangef(<float>nearVal, <float>farVal)


--void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers)
--glDeleteRenderbuffers(<int>n, {[int]}renderbuffers)


--void glVertexAttrib1f(GLuint index, GLfloat v0)
--glVertexAttrib1f(<int>index, <float>v0)


--void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
--glVertexAttrib2f(<int>index, <float>v0, <float>v1)


--void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
--glVertexAttrib3f(<int>index, <float>v0, <float>v1, <float>v2)


--void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
--glVertexAttrib4f(<int>index, <float>v0, <float>v1, <float>v2, <float>v3)


--void glVertexAttrib1fv(GLuint index, const GLfloat * v)
--glVertexAttrib1fv(<int>index, {[float]}v)


--void glVertexAttrib2fv(GLuint index, const GLfloat * v)
--glVertexAttrib2fv(<int>index, {[float]}v)


--void glVertexAttrib3fv(GLuint index, const GLfloat * v)
--glVertexAttrib3fv(<int>index, {[float]}v)


--void glVertexAttrib4fv(GLuint index, const GLfloat * v)
--glVertexAttrib4fv(<int>index, {[float]}v)


--void glBindAttribLocation(GLuint program, GLuint index, const GLchar * name)
--glBindAttribLocation(<int>program, <int>index, <string>name)


--GLint glGetUniformLocation(GLuint program, const GLchar * name)
--local <int>_ll_ret = glGetUniformLocation(<int>program, <string>name)


--void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params)
--local <float>params = glGetTexParameterfv(<int>target, <int>pname)


--void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params)
--local <int>params = glGetTexParameteriv(<int>target, <int>pname)


--void glLineWidth(GLfloat width)
--glLineWidth(<float>width)


--void glGetProgramiv(GLuint program, GLenum pname, GLint * params)
--local <int>params = glGetProgramiv(<int>program, <int>pname)


--GLboolean glIsFramebuffer(GLuint framebuffer)
--local <bool>_ll_ret = glIsFramebuffer(<int>framebuffer)


--void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
--glCopyTexImage2D(<int>target, <int>level, <int>internalformat, <int>x, <int>y, <int>width, <int>height, <int>border)


--void glDetachShader(GLuint program, GLuint shader)
--glDetachShader(<int>program, <int>shader)


--void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params)
--local <int>params = glGetRenderbufferParameteriv(<int>target, <int>pname)


--void glPolygonOffset(GLfloat factor, GLfloat units)
--glPolygonOffset(<float>factor, <float>units)


--GLboolean glIsProgram(GLuint program)
--local <bool>_ll_ret = glIsProgram(<int>program)


--void glGetBufferParameteriv(GLenum target, GLenum value, GLint * data)
--local <int>data = glGetBufferParameteriv(<int>target, <int>value)


--void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
--glRenderbufferStorage(<int>target, <int>internalformat, <int>width, <int>height)


--void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source)
--local <int>length, <string>source = glGetShaderSource(<int>shader, <int>bufSize)


--void glHint(GLenum target, GLenum mode)
--glHint(<int>target, <int>mode)


--void glValidateProgram(GLuint program)
--glValidateProgram(<int>program)


--void glStencilMask(GLuint mask)
--glStencilMask(<int>mask)


--GLboolean glIsBuffer(GLuint buffer)
--local <bool>_ll_ret = glIsBuffer(<int>buffer)


--GLboolean glIsShader(GLuint shader)
--local <bool>_ll_ret = glIsShader(<int>shader)


--void glClearDepthf(GLclampf depth)
--glClearDepthf(<float>depth)


--void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data)
--glTexSubImage2D(<int>target, <int>level, <int>xoffset, <int>yoffset, <int>width, <int>height, <int>format, <int>type, <string>data)


--void glDeleteTextures(GLsizei n, const GLuint * textures)
--glDeleteTextures(<int>n, {[int]}textures)


--void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog)
--local <int>length, <string>infoLog = glGetShaderInfoLog(<int>shader, <int>maxLength)


--void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
--glBlendFuncSeparate(<int>srcRGB, <int>dstRGB, <int>srcAlpha, <int>dstAlpha)


--void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)
--local <int>length, <int>size, <int>type, <string>name = glGetActiveAttrib(<int>program, <int>index, <int>bufSize)


--void glBlendFunc(GLenum sfactor, GLenum dfactor)
--glBlendFunc(<int>sfactor, <int>dfactor)


--void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei * length, GLchar * infoLog)
--local <int>length, <string>infoLog = glGetProgramInfoLog(<int>program, <int>maxLength)


--void glAttachShader(GLuint program, GLuint shader)
--glAttachShader(<int>program, <int>shader)


--void glPixelStorei(GLenum pname, GLint param)
--glPixelStorei(<int>pname, <int>param)

