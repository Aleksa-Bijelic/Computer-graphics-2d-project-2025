#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 uPos;
uniform vec2 uScale;

void main() {
    gl_Position = vec4(aPos.x * uScale.x + uPos.x, aPos.y * uScale.y + uPos.y, 0.0, 1.0);
}
