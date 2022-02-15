//-----------------------------------------------------------------------------
// File Name:    Renderer.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

#include "Renderer.h"

#include "Affine.h"
#include "Transform.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "SpriteFont.h"
#include "ParticleSystem.h"
#include "References.h"
#include "Shader.h"

#include "DebugDraw.h"
#include "PhysicsSystem.h"
#include "Hitbox.h"
#include "RigidBody.h"

#include <algorithm>

#include "Debuggers.h"
#include "Input.h"

Shader Renderer::textureShader;
Shader Renderer::colorShader;

// Camera Renderer::mainCamera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
//  glViewport(0, 0, width, height);

  Renderer::Inst()->ResizeWindow(width, height);

  Renderer::Inst()->mainCamera->ResizeWindow(width, height);
}



//-----------------------------------------------------------------------------
// Constructors and Destructors
//-----------------------------------------------------------------------------
Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

//-----------------------------------------------------------------------------
// ISystem methods
//-----------------------------------------------------------------------------
void Renderer::Start()
{

  // Getting GLFW window dependency
  window = GLFWSystem::Inst()->GetGLFWWindow();

  // For window readjustment
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Loads Shaders
  LoadShaders();


  // Camera Setup
  int w, h;
  vec4 O = Affine::Point(0, 0);

  glfwGetWindowSize(window, &w, &h);

  // Enabling Culling
  glDisable(GL_CULL_FACE);


  aimingLine.p0 = vec3(INFINITY, INFINITY, INFINITY);
  aimingLine.p1 = vec3(INFINITY, INFINITY, INFINITY);

  aimingLineTransform = NULL;

  SetPauseStatus(false);
}

void Renderer::Update( float dt )
{

	Draw();

}

void Renderer::Shutdown()
{
  glUseProgram(0);

}

void Renderer::AddSpriteRenderer( const SpriteRendererPtr spriteRenderer )
{
  _spriteRenderers.push_back(spriteRenderer);
}

void Renderer::RemoveSpriteRenderer( const SpriteRendererPtr spriteRenderer )
{
//  if (_spriteRenderer.)

  _spriteRenderers.erase( std::remove(_spriteRenderers.begin(), _spriteRenderers.end(), spriteRenderer) );
}

void Renderer::AddParticleSystem( const ParticleSystemPtr particleSystem )
{
  _particleRenderers.push_back(particleSystem);
}

void Renderer::RemoveParticleSystem( const ParticleSystemPtr particleSystem)
{
  _particleRenderers.erase( std::find( _particleRenderers.begin(), _particleRenderers.end(), particleSystem ) );
}


void Renderer::ResizeWindow( int w, int h )
{
  glViewport(0, 0, w, h);

  mainCamera->ResizeWindow(w, h);

}

void Renderer::AddCamera( const CameraPtr camera )
{
  cameras_.push_back( camera );

  if (!mainCamera)
	  mainCamera = cameras_.front();
}

void Renderer::RemoveCamera( const CameraPtr camera )
{
  if (camera == mainCamera)
  {
	  cameras_.remove( camera );

    if (!cameras_.empty())
      mainCamera = cameras_.front();
	  else
		  mainCamera = nullptr;

	  return;
  }

	cameras_.remove( camera );

}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------
void Renderer::clearFrame( const glm::vec4& c )
{
  glClearColor(c.x, c.y, c.z, c.w);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetModelingTransform( const glm::mat4& M )
{
  glUniformMatrix4fv(utransform, 1, false, &M[0][0]);
}



// ----------------------------------------------------------------------------
// Debug Stuff
// ----------------------------------------------------------------------------
//void Renderer::DrawLine( glm::vec2 pt1, glm::vec2 pt2, glm::vec4 color )
//{
//  debugLines.push_back(pt1);
//  debugLines.push_back(pt2);
//  debugLinesColor = color;
//}



//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------
void Renderer::LoadShaders()
{
	Shader tempShader("Shaders/tex_vertex_shader.vert", "Shaders/tex_fragment_shader.frag");
	textureShader = tempShader;

	Shader tempColorShader("Shaders/col_vertex_shader.vert", "Shaders/col_fragment_shader.frag");
	colorShader = tempColorShader;
}

void Renderer::UpdateGLFW()
{
  glfwSwapBuffers(GLFWSystem::Inst()->GetGLFWWindow());
  glfwPollEvents();
}


static void SafeDeleteBuffer(GLuint & buffer)
{
	if (buffer != 0)
	{
		glDeleteBuffers(1, &buffer);

		buffer = 0;
	}
}

bool Renderer::RenderSpriteFont(const SpriteRendererPtr spriteRenderer)
{
	if (spriteRenderer->Text() != NULL)
	{
		mat4 worldToNDC = mainCamera->CameraToNDC() * mainCamera->WorldToCamera();
		mat4 objectToNDC;

		const SpriteFontPtr spriteFontObj = spriteRenderer->Text();

		spriteFontObj->UpdateCharInfo();

		const std::vector<std::pair<SpriteFont::CharTexCoords, TransformPtr>> * char_info = spriteFontObj->GetCharInfo();

		std::vector<std::pair<SpriteFont::CharTexCoords, TransformPtr>>::const_iterator char_iter;

		for (char_iter = char_info->begin(); char_iter != char_info->end(); char_iter++)
		{
			Mesh * texturedSquareMesh = spriteRenderer->Mesh();
			//LoadTextureMesh(texturedSquareMesh);
			float start_u = (*char_iter).first.start_u;
			float start_v = (*char_iter).first.start_v;
			float u = (*char_iter).first.u;
			float v = (*char_iter).first.v;


			float temp = start_u;
			start_u = u;
			u = temp;

			texturedSquareMesh->texCoords.clear();
			texturedSquareMesh->texCoords.push_back(vec2(start_u, start_v));
			texturedSquareMesh->texCoords.push_back(vec2(u, start_v));
			texturedSquareMesh->texCoords.push_back(vec2(u, v));
			texturedSquareMesh->texCoords.push_back(vec2(start_u, v));

			((*char_iter).second)->UpdateTransform();

			objectToNDC = worldToNDC * ((*char_iter).second)->Matrix();

			textureShader.SetUniform("transform", objectToNDC);
			glActiveTexture(GL_TEXTURE0);
			textureShader.SetUniform("texture_", 0);
			glBindTexture(GL_TEXTURE_2D, (spriteRenderer)->Texture()->GetTextureID());

			Mesh * _mesh = (spriteRenderer)->Mesh();
			_mesh->RebindTexCoords();
			_mesh->Draw(_mesh, textureShader/*, _spriteColor*/);


			glActiveTexture(GL_TEXTURE0);

		}

		return true;
	}

	return false;
}

void Renderer::Draw()
{

  // clear frame buffer
  clearFrame(glm::vec4(0, 0, 0, 1));
  glClear(GL_COLOR_BUFFER_BIT);

    // Exit if mainCamera doesn't exist
  if (!mainCamera)
	  return;

  // Camera Transforms
  mat4 worldToNDC = mainCamera->CameraToNDC() * mainCamera->WorldToCamera();

  textureShader.Use();

  // Drawing each textured mesh
  DrawSprites(&worldToNDC);
  //---------------------------
  DrawParticles(&worldToNDC);
  //---------------------------
  textureShader.UnUse();

  DrawDebugDraw(&worldToNDC);

  RenderAimingLine(&worldToNDC);

}

void Renderer::DrawBackground()
{

}


void Renderer::DrawSprites(mat4 * worldToNDC)
{
	std::sort(_spriteRenderers.begin(), _spriteRenderers.end(), CompareRenderLayers());

	for (_SRIter = _spriteRenderers.begin(); _SRIter != _spriteRenderers.end(); _SRIter++)
	{
		// Destroy rogue spriteRenderer
		if (!(*_SRIter)->Parent())
		{
			// delete _SRIter;
			_spriteRenderers.erase(_SRIter);
			continue;
		}


		if (!(*_SRIter)->IsEnabled() || !(*_SRIter)->Texture()) continue;


		if (!(RenderSpriteFont(*_SRIter)))
		{
			Mesh* texturedSquareMesh = (*_SRIter)->Mesh();

	   	PrintGLError("Renderer: Mesh Draw");

			texturedSquareMesh->texCoords.clear();
			texturedSquareMesh->texCoords.push_back(vec2(1.0f, 0.0f));
			texturedSquareMesh->texCoords.push_back(vec2(0.0f, 0.0f));
			texturedSquareMesh->texCoords.push_back(vec2(0.0f, 1.0f));
			texturedSquareMesh->texCoords.push_back(vec2(1.0f, 1.0f));

			mat4 objectToNDC = *worldToNDC * (*_SRIter)->Parent()->GetTransform().Matrix();

			 	PrintGLError("Renderer: Mesh Draw");

			//SetModelingTransform(objectToNDC);
			textureShader.SetUniform("transform", objectToNDC);
			glActiveTexture(GL_TEXTURE0);

      PrintGLError("Renderer: Mesh Draw");

			textureShader.SetUniform("texture_", 0);

			PrintGLError("Renderer: Mesh Draw");

			GLuint texture_id = (*_SRIter)->Texture()->GetTextureID();

			//std::cout << texture_id << std::endl;

			glBindTexture(GL_TEXTURE_2D, texture_id);

	  	PrintGLError("Renderer: Mesh Draw");


			texturedSquareMesh->RebindTexCoords();
			texturedSquareMesh->Draw(texturedSquareMesh, textureShader/*, _spriteColor*/);

			glActiveTexture(GL_TEXTURE0);

		}

	}
}


void Renderer::DrawParticles(mat4 * worldToNDC)
{
	for (_PSIter = _particleRenderers.begin(); _PSIter != _particleRenderers.end(); _PSIter++)
	{
		// Destroy rogue spriteRenderer
		if (!(*_PSIter)->Parent())
		{
			// delete _PSIter;
			_particleRenderers.erase(_PSIter);
			continue;
		}

		if (!(*_PSIter)->GetTexture()) continue;

		std::deque<Particle> particles;

		(*_PSIter)->GetParticles(particles);

		for (auto particle : particles)
		{
			// Skip dead part
			if (particle.remainingLifetime <= 0.0f)
			{
				continue;
			}

			mat4 objectToNDC = *worldToNDC *
				(glm::translate(mat4(1.0f), vec3(particle.position, 0.0f)) *
					glm::scale(mat4(1.0f), vec3(particle.startSize, 0.0f)));


			textureShader.SetUniform("transform", objectToNDC);
			glActiveTexture(GL_TEXTURE0);
			textureShader.SetUniform("texture_", 0);
			glBindTexture(GL_TEXTURE_2D, (*_PSIter)->GetTexture()->GetTextureID());

			Mesh * _mesh = Mesh::GetTexturedMesh();
			_mesh->Draw(_mesh, textureShader/*, _spriteColor*/);



			glActiveTexture(GL_TEXTURE0);

		}
	}
}


void Renderer::DrawDebugDraw(mat4 * worldToNDC)
{
	if (DebugDraw::Inst()->GetShowHitbox() || DebugDraw::Inst()->GetShowKinematics())
	{
		colorShader.Use();


		const std::vector<RigidBodyPtr> * physicsListCopy = Physics::Inst()->GetVector();
		unsigned int physicsListCopy_size = Physics::Inst()->GetVectorSize();

		for (unsigned int list_pos = 0; list_pos < physicsListCopy_size; list_pos++)
		{
			HitboxPtr objectHitbox = (*physicsListCopy)[list_pos]->Parent()->GetComponent<Hitbox>(Component::cHitbox);
			Hitbox::HitboxShape objectHitbox_shape = objectHitbox->GetShape();


			  // Draw hitboxes
			mat4 objectToNDC = *worldToNDC * (*physicsListCopy)[list_pos]->Parent()->GetTransform().Matrix();
			colorShader.SetUniform("transform", objectToNDC);

			if (DebugDraw::Inst()->GetShowHitbox())
			{
				if (objectHitbox_shape == Hitbox::cCircle)
				{
					Mesh* _mesh = Mesh::GetCircleLoopMesh();
					_mesh->Draw(_mesh, colorShader);
				}
				else if (objectHitbox_shape == Hitbox::cBoxlike)
				{
					Mesh* _mesh = Mesh::GetSquareLoopMesh();
					_mesh->Draw(_mesh, colorShader);
				}
			}


			  // Draw velocity vectors
			if (DebugDraw::Inst()->GetShowKinematics())
			{
				vec3 velocity = *((*physicsListCopy)[list_pos]->GetVelocity());

				Mesh* __mesh = Mesh::GetLineMesh();
				__mesh->vertices[1] = vec2(__mesh->vertices[0].x + velocity.x, __mesh->vertices[0].y + velocity.y);
				__mesh->RebindVertexBuffer();
				__mesh->Draw(__mesh, colorShader);
			}


		}

		colorShader.UnUse();
	}
}

void Renderer::DisplayAimingLineLua(vec2 origin_point, vec2 direction, TransformPtr transform, float radius)
{
	Renderer::Inst()->DisplayAimingLine(origin_point, direction, transform, radius);
}

void Renderer::DisplayAimingLine(vec2 origin_point, vec2 direction, TransformPtr transform, float radius)
{
	vec2 dir_norm = normalize(direction);
	vec2 outside_point = origin_point + ((radius + 1.0f) * dir_norm);

	RaycastHitData hitData = Physics::Inst()->Raycast(outside_point, direction, INFINITY, transform->Parent());

	vec2 end_point = origin_point + (1000.0f * dir_norm);

	if (hitData.gameobject != nullptr)
	{
		end_point = hitData.point;
	}

	aimingLine.p0 = vec3(origin_point, 1.0f);
	aimingLine.p1 = vec3(end_point, 1.0f);

	aimingLineTransform = transform;
}

void Renderer::RenderAimingLine(mat4* worldToNDC)
{
	if (aimingLine.p0.x != INFINITY)
	{
		colorShader.Use();


		mat4 objectToNDC = *worldToNDC * aimingLineTransform->Matrix();
		colorShader.SetUniform("transform", objectToNDC);

		vec2 line_vec = aimingLine.p1 - aimingLine.p0;

		Mesh* mesh = Mesh::GetLineMesh();
		mesh->vertices[1] = vec2(mesh->vertices[0].x + line_vec.x, mesh->vertices[0].y + line_vec.y);
		mesh->RebindVertexBuffer();
		mesh->Draw(mesh, colorShader);

		colorShader.UnUse();

		aimingLine.p0 = vec3(INFINITY, INFINITY, INFINITY);
		aimingLine.p1 = vec3(INFINITY, INFINITY, INFINITY);

		aimingLineTransform = NULL;
	}
}

void Renderer::CreateUI()
{
	GameObjectPtr UI_life = new GameObject("heck");

	TexturePtr texture__ = new Texture(References::FilePaths::TEXTURES_FOLDER + "Life.png");
	SpriteRenderer* sprite_renderer__ = new SpriteRenderer(texture__);
	sprite_renderer__->SetRenderLayer(layerUI);
	sprite_renderer__->Mesh(Mesh::GetTexturedMesh());

	UI_life->AddComponent(sprite_renderer__);

	UI_life->transform->Scale(vec2(0.5f, 0.5f));
	UI_life->transform->Position(mainCamera->Parent()->transform->Position());
}
