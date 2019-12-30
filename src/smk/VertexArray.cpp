// Copyright 2019 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <smk/VertexArray.hpp>

namespace smk {

VertexArray::VertexArray() = default;

void VertexArray::Allocate(int element_size, void* data) {
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  glBufferData(GL_ARRAY_BUFFER, size_ * element_size, data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
}

VertexArray::~VertexArray() {
  if (!vbo_)
    return;

  if (ref_count_) {
    --(*ref_count_);
    if (*ref_count_)
      return;
    delete ref_count_;
    ref_count_ = nullptr;
  }

  glDeleteBuffers(1, &vbo_);
  vbo_ = 0;

  glDeleteVertexArrays(1, &vao_);
  vao_ = 0;
}

void VertexArray::Bind() const {
  glBindVertexArray(vao_);
}

void VertexArray::UnBind() const {
  glBindVertexArray(0);
}

VertexArray::VertexArray(const VertexArray& other) {
  this->operator=(other);
}

VertexArray::VertexArray(VertexArray&& other) {
  this->operator=(std::move(other));
}

VertexArray& VertexArray::operator=(const VertexArray& other) {
  this->~VertexArray();
  if (!other.vbo_)
    return *this;

  if (!other.ref_count_)
    other.ref_count_ = new int(1);

  vbo_ = other.vbo_;
  vao_ = other.vao_;
  ref_count_ = other.ref_count_;
  size_ = other.size_;

  (*ref_count_)++;
  return *this;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
  std::swap(vbo_, other.vbo_);
  std::swap(vao_, other.vao_);
  std::swap(size_, other.size_);
  std::swap(ref_count_, other.ref_count_);
  return *this;
}

VertexArray::VertexArray(const std::vector<Vertex2D>& array) {
  size_ = array.size();
  Allocate(sizeof(Vertex2D), (void*)array.data());
  Vertex2D::Bind();
}

VertexArray::VertexArray(const std::vector<Vertex3D>& array) {
  size_ = array.size();
  Allocate(sizeof(Vertex3D), (void*)array.data());
  Vertex3D::Bind();
}

}  // namespace smk.
