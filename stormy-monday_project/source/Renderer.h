//-----------------------------------------------------------------------------
// File Name:    Renderer.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include <list>

#include "GLFWSystems.h"
#include "imgui.h"
#include "ISystem.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>
#include "Hitbox.h"
#include "SpriteRenderer.h"

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class Camera Camera, *CameraPtr;
class Mesh;
typedef class SpriteRenderer SpriteRenderer, *SpriteRendererPtr;
typedef class ParticleSystem ParticleSystem, *ParticleSystemPtr;
typedef class Texture Texture, *TexturePtr;
typedef class Shader Shader;

class TexturedSquareMesh;



class Renderer : public ISystem
{
public:
  Renderer();
  ~Renderer();

  // Singleton call
  static Renderer* Inst()
  {
    static Renderer inst;
    return &inst;
  }

  // ISystem Methods
  void Start() override;
  void Update(float dt) override;
  void Shutdown() override;

  void Draw();

  // Public Methods
  void ResizeWindow( int w, int h );

  static void AddCamera( const CameraPtr camera);
  static void RemoveCamera( const CameraPtr camera);

  void AddSpriteRenderer( const SpriteRendererPtr spriteRenderer );
  void RemoveSpriteRenderer( const SpriteRendererPtr spriteRenderer );

  void AddParticleSystem(const ParticleSystemPtr particleSystem);
  void RemoveParticleSystem(const ParticleSystemPtr particleSystem);

 
  bool RenderSpriteFont(const SpriteRendererPtr spriteRenderer);


  void clearFrame(const glm::vec4 &c);
  void SetModelingTransform(const glm::mat4 &M);
  void loadMesh(const Mesh &m);
  void unloadMesh(void);
  void LoadTextureMesh(const TexturedSquareMesh& squareTexture);
  void UnloadTextureMesh(const TexturedSquareMesh& squareTexture);
  static void LoadTexture(Texture& texture);
  void UnloadTexture(Texture& texture);
  void displayEdges(const glm::vec4 &c);
  void displayFaces(const glm::vec4 &c);
  void DisplayTexture(const SpriteRenderer*);
  void DisplayTexture(Texture& texture);

  static void DisplayAimingLineLua(vec2 origin_point, vec2 direction, TransformPtr transform, float radius);
  void DisplayAimingLine(vec2 origin_point, vec2 direction, TransformPtr transform, float radius);

  void RenderAimingLine(mat4* worldToNDC);


  // Debug Stuff
  //void DrawLine(glm::vec2 pt1, glm::vec2 pt2, glm::vec4 color = glm::vec4(1, 0, 0, 1));
  void DrawDebugLine(vec2 pt1, vec2 pt2);
  void DrawDebugCircle(vec3 position, float radius);

  // inline static GameObjectPtr mainCameraGO = nullptr;
  inline static CameraPtr mainCamera = nullptr;

  static Shader textureShader;
  static Shader colorShader;

private:
  
  GLFWwindow* window;

  GLint  ucolor,
         utransform,
         aposition,
         atexcoord,
         udebugColor;
  /*
  GLuint vao,
         dao,
         bgao,
         program,
         vertex_buffer,
         edge_buffer,
         face_buffer,*/
  GLuint texcoord_buffer = 0;

  /*int    mesh_edge_count,
         mesh_face_count;*/


//  std::vector<glm::mat4> transformMatrices;

  inline static std::list<CameraPtr> cameras_;
  std::vector<SpriteRendererPtr> _spriteRenderers;
  std::vector<ParticleSystemPtr> _particleRenderers;
  std::vector<SpriteRendererPtr>::iterator _SRIter;
  std::vector<ParticleSystemPtr>::iterator _PSIter;


  //Texture bkg;

  //Texture line;


  
  // Debug stuff
  std::vector<glm::vec2> debugLines;
  glm::vec4 debugLinesColor;

  Hitbox::HitboxLine aimingLine;
  TransformPtr aimingLineTransform;

  // Private Methods
  void LoadShaders();
  void UpdateGLFW();
//  void Draw(float dt);
//  void Draw(const glm::mat4& matrix);
  
  // Draws all sprites onto the screen
  void DrawSprites(mat4 * worldToNDC);
  void DrawParticles(mat4 * worldToNDC);
  void DrawDebugDraw(mat4 * worldToNDC);

  // 
  void DrawBackground();
//  void DrawDebugLines();


  void CreateUI();

  

};

struct CompareRenderLayers
{
	bool operator() (SpriteRenderer * rend1, SpriteRenderer * rend2) const
	{
		return (rend1->GetRenderLayer() < rend2->GetRenderLayer());
	}
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height);


