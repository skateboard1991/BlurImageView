//
// Created by wujiaxin1 on 2018/8/22.
//
#include <jni.h>
#include <string>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define  LOG_TAG    "blurimagejni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
using namespace std;
extern "C"
{

unsigned int texture;
unsigned int VAO, VBO;
unsigned int FBUFFERS[2];
unsigned int FBUFFERTEXTURE[2];
int program;

float pos[] = {-1.0f, -1.0f, 0.0f, 1.0f,
               1.0f, 1.0f, 1.0f, 0.0f,
               -1.0f, 1.0f, 0.0f, 0.0f,
               -1.0f, -1.0f, 0.0f, 1.0f,
               1.0f, -1.0f, 1.0f, 1.0f,
               1.0f, 1.0f, 1.0f, 0.0f};

int createShader(string vertex, int type) {
    const char *shaderSourceP = vertex.c_str();
    int shader = glCreateShader(type);
    if (!shader) {
        LOGE("create shader failed\n");
    }
    glShaderSource(shader, 1, &shaderSourceP, nullptr);
    glCompileShader(shader);
    int success = 0;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    {
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            LOGE("compile shader failed %s \n", infoLog);
        }
    }
    return shader;

}

void generateProgram(JNIEnv *env, jstring vertex, jstring fragment) {
    const char *cVertex = env->GetStringUTFChars(vertex, nullptr);
    const char *cFragment = env->GetStringUTFChars(fragment, nullptr);
    int vertexShader = createShader(cVertex, GL_VERTEX_SHADER);
    int fragmentShader = createShader(cFragment, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    if (!program) {
        LOGE("create program failed\n");
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int success = 0;
    char info[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, info);
        LOGE("create program failed %s \n", info);
    }
}

void prepareFrameBuffer(int width, int height) {

    glGenFramebuffers(2, FBUFFERS);
    glGenTextures(2, FBUFFERTEXTURE);
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBUFFERS[i]);
        glBindTexture(GL_TEXTURE_2D, FBUFFERTEXTURE[i]);
        glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBUFFERTEXTURE[i], 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("frame buffer not completed");
        }
    }

}


void prepareTexture(JNIEnv *env, jobject bitmap) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    AndroidBitmapInfo info;
    void *pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        LOGE("bitmap is wrong");
    } else {
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) == 0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         pixels);
        } else {
            LOGE("lock bitmap error");
        }
        AndroidBitmap_unlockPixels(env, bitmap);
    }
}


void prepareVertex() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    glBindVertexArray(0);

}

JNIEXPORT void JNICALL
Java_com_skateboard_blurimageview_BlurImageViewRender_prepare(JNIEnv *env, jobject thiz,
                                                              jstring vertex, jstring fragment,
                                                              jobject bitmap,jint srcWidth,jint srcHeight) {
    generateProgram(env, vertex, fragment);
    prepareVertex();
    prepareTexture(env, bitmap);
    prepareFrameBuffer(srcWidth,srcHeight);
}

void setMatrix() {
    int modelLocation = glGetUniformLocation(program, "model");
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
    int viewLocation = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    int projectionLocation = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

JNIEXPORT void JNICALL
Java_com_skateboard_blurimageview_BlurImageViewRender_draw(JNIEnv *env, jobject thiz) {
    int isVertical = 0;
    bool isFirst = true;
    glUseProgram(program);
    for (int i = 0; i < 12; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBUFFERS[isVertical]);
        int isVerticalLocation = glGetUniformLocation(program, "isVertical");
        glUniform1i(isVerticalLocation, isVertical);
        if (isFirst) {
            glBindTexture(GL_TEXTURE_2D, texture);
            isFirst = false;
        } else {
            glBindTexture(GL_TEXTURE_2D, FBUFFERTEXTURE[!isVertical]);
        }
        glBindVertexArray(VAO);
        int modelLocation = glGetUniformLocation(program, "model");
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        isVertical = !isVertical;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);
    setMatrix();
    glBindTexture(GL_TEXTURE_2D, FBUFFERTEXTURE[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


}
