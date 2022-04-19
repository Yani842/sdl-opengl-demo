#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// every object has its own vbo in animationatate and the rendering eill be like
// before opengl(no bind-unbind vao etc) render method will just go threw the
// objects its given end render them by their vbo from animationstate also every
// oonject will have its own texture id and before drawing it it will bind that
// texture *or* ill succsfully apload a texture arraay to the shader and will
// call them by index - much more eficient and i did a lot abt it

Render::Render() {}

unsigned int Render::loadTexture(const char* path) {
  string strPath = "../Data/Images/";
  strPath += path;
  unsigned int tex;

  glActiveTexture(GL_TEXTURE0);

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(strPath.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, 1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, 0);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA,
                    GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    // glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, sizeX, sizeY, 6, 0,
    // GL_RGBA, GL_UNSIGNED_BYTE, None)
    // for i in range(6):
    // glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, sizeX, sizeY, 1,
    // GL_RGBA, GL_UNSIGNED_BYTE, image_planes[i])

  } else {
    cout << "Failed to load texture\n";
  }
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // glBindTexture(GL_TEXTURE_2D, tex);
  // string uniform = "textures[";
  // uniform += textureNum;
  // uniform += "]";
  // shader.setInt("textures", textureNum);
  textureNum++;
  stbi_image_free(data);
  return tex;
}

void Render::addAnimation(float rate, int texIndex, int textures,
                          bool noAnimation, bool oneCycle) {
  Animation an;
  an.textureIndex = texIndex;
  an.textures = textures;
  an.rate = rate;
  an.noAnimation = noAnimation;
  an.oneCycle = oneCycle;
  animations.push_back(an);
}

void Render::init() {
  shader.init("../Data/Shaders/shader.vs", "../Data/Shaders/shader.fs");
  shader.use();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(255, VBOs);

  glBindVertexArray(VAO);

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::loadAnimations() {
  animations.clear();
  loadTexture("ghost/idle-1.png");
  addAnimation(0, 0, 1, true);
  loadTexture("ghost/left-1.png");
  addAnimation(0, 1, 1, true);
  loadTexture("ghost/right-1.png");
  addAnimation(0, 2, 1, true);
  loadTexture("ground/grass-1.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("ground/grass-2.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("ground/grass-3.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("ground/grass-4.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("ground/grass-5.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("ground/grass+water.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("foliage/tree-1.png");
  addAnimation(0, textureNum - 1, 1, true);
  loadTexture("foliage/tree-2.png");
  addAnimation(0, textureNum - 1, 1, true);
}

void Render::bind(vector<shared_ptr<Object>>* objects, unsigned char layer) {
  // glBufferData( GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW );
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[layer]);
  Vertex vertices[objects->size() * 4];
  // adding all object's vertices to one array
  for (int i = 0; i < objects->size(); i++) {
    for (int j = 0; j < 4; j++) {
      vertices[i * 4 + j] = objects->at(i)->anState.vertices[j];
    }
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // pos attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  // texcoord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texIndex attribute
  glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                        (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::updateLayer(unsigned char layer, unsigned int objIndex,
                         Vertex vertices[4]) {
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[layer]);
  cout << vertices[0].texIndex << vertices[1].texIndex << vertices[2].texIndex
       << vertices[3].texIndex << "\n";
  glBufferSubData(GL_ARRAY_BUFFER, objIndex * sizeof(Vertex) * 4,
                  sizeof(Vertex) * 4, vertices);
  // pos attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  // texcoord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texIndex attribute
  glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                        (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::unbind(unsigned char layer) { glDeleteBuffers(1, &VBOs[layer]); }

void Render::update(float dt, vector<shared_ptr<Object>>* objects) {
  for (auto obj : *objects) {
    if (obj == NULL) {
      continue;
    }
    auto* st = &obj->anState;
    auto* an = &animations.at(st->animation);
    if (st->didAnimationEnded() || an->noAnimation) {
      continue;
    }
    st->pointInRate += dt;
    if (st->pointInRate > an->rate) {
      st->pointInRate = 0;
      st->currentFrame++;
      if (st->currentFrame >= an->textures) {
        st->currentFrame = 0;
        if (an->oneCycle) {
          st->animationEnded = true;
        }
      }
    }
  }
}

void Render::render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

// vbo for every layer(not all bind at the same time)