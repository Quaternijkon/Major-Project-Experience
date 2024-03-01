#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <iostream>

// 窗口尺寸设置
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 720;

// 照相机实例化
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 计时
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


glm::vec3 spotLightColor(1.0f, 1.0f, 1.0f);

float spotLighting = 1.0f;

// 检测W，S，A，D，Esc键，方向键的按下与释放，并做出响应
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        if (spotLighting - deltaTime * 0.5 > 0)
            spotLighting -= deltaTime * 0.5;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        spotLighting += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (spotLightColor.x + deltaTime * 0.2 > 1.0)
            spotLightColor.x += deltaTime * 0.2 - 1.0;
        else
            spotLightColor.x += deltaTime * 0.2;
        if (spotLightColor.y + deltaTime * 0.3 > 1.0)
            spotLightColor.y += deltaTime * 0.3 - 1.0;
        else
            spotLightColor.y += deltaTime * 0.3;
        if (spotLightColor.z + deltaTime * 0.5 > 1.0)
            spotLightColor.z += deltaTime * 0.5 - 1.0;
        else
            spotLightColor.z += deltaTime * 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (spotLightColor.x - deltaTime * 0.2 < 0.0)
            spotLightColor.x -= deltaTime * 0.2 + 1.0;
        else
            spotLightColor.x -= deltaTime * 0.2;
        if (spotLightColor.y - deltaTime * 0.3 < 0.0)
            spotLightColor.y -= deltaTime * 0.3 + 1.0;
        else
            spotLightColor.y -= deltaTime * 0.3;
        if (spotLightColor.z - deltaTime * 0.5 < 0.0)
            spotLightColor.z -= deltaTime * 0.5 + 1.0;
        else
            spotLightColor.z -= deltaTime * 0.5;
    }
}

// 检测窗口尺寸是否发生变化
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // 确保视口与新窗口尺寸匹配；宽度和高度将明显大于显示器上的指定值。
    glViewport(0, 0, width, height);
}

// 当鼠标移动时回调
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 当y坐标从底部到顶部时反转

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// 当鼠标滚轮操作时回调
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 用于从文件加载2D纹理的函数
unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void GlfwInit() {// glfw的初始化和配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *GlfwWindow() {// glfw窗口创建
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
//    if (window == nullptr) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
}

// 设置顶点数据和缓冲区并配置顶点属性
float vertices[] = {

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

// 所有箱子的位置信息
glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};
// 点光源的位置信息
glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f),
};

void CubaSetting(unsigned int &VBO, unsigned int &cubeVAO) {// 首先配置立方体的VAO和VBO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int LightSetting() {// 其次,配置光源的VAO， VBO保持不变; 光源也是顶点组成的立方体。
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    return lightCubeVAO;
}

void LoadTextture(unsigned int &diffuseMap, unsigned int &specularMap) {
    diffuseMap = loadTexture("../resources/textures/container2.png");
    specularMap = loadTexture("../resources/textures/container2_specular.png");// 加载纹理
}

void ShadeSetting(const Shader &lightingShader) {
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
}

void ActiveShade(const Shader &lightingShader) {
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("lightPos", lightPos);
}

void DirLight(const Shader &lightingShader) {// 平行光
    lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.1f);
    lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.7);
    lightingShader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);
}

void PrtLight(const Shader &lightingShader, const glm::vec3 *pointLightColors) {// 点光源1
    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x + lightPos.x,
                           pointLightPositions[0].y + lightPos.y,
                           pointLightPositions[0].z);
    lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1,
                           pointLightColors[0].z * 0.1);
    lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y,
                           pointLightColors[0].z);
    lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y,
                           pointLightColors[0].z);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09f);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
    // 点光源2
    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x + lightPos.x,
                           pointLightPositions[1].y + lightPos.y,
                           pointLightPositions[1].z);
    lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1,
                           pointLightColors[1].z * 0.1);
    lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y,
                           pointLightColors[1].z);
    lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y,
                           pointLightColors[1].z);
    lightingShader.setFloat("pointLights[1].constant", 1.0f);
    lightingShader.setFloat("pointLights[1].linear", 0.09f);
    lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
    // 点光源3
    lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x + lightPos.x,
                           pointLightPositions[2].y + lightPos.y,
                           pointLightPositions[2].z);
    lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1,
                           pointLightColors[2].z * 0.1);
    lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y,
                           pointLightColors[2].z);
    lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y,
                           pointLightColors[2].z);
    lightingShader.setFloat("pointLights[2].constant", 1.0f);
    lightingShader.setFloat("pointLights[2].linear", 0.09f);
    lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
    // 点光源4
    lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x + lightPos.x,
                           pointLightPositions[3].y + lightPos.y,
                           pointLightPositions[3].z);
    lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1,
                           pointLightColors[3].z * 0.1);
    lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y,
                           pointLightColors[3].z);
    lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y,
                           pointLightColors[3].z);
    lightingShader.setFloat("pointLights[3].constant", 1.0f);
    lightingShader.setFloat("pointLights[3].linear", 0.09f);
    lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
}

void SpotLight(const Shader &lightingShader) {// 聚光灯
    lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
    lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
    lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightingShader.setVec3("spotLight.diffuse", spotLightColor.x, spotLightColor.y, spotLightColor.z);
    lightingShader.setVec3("spotLight.specular", spotLightColor.x, spotLightColor.y, spotLightColor.z);
    lightingShader.setFloat("spotLight.constant", spotLighting);
    lightingShader.setFloat("spotLight.linear", 0.009f);
    lightingShader.setFloat("spotLight.quadratic", 0.0032f);
    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
}

void RenderObject(const Shader &lightingShader, unsigned int cubeVAO) {// 渲染对象
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // 计算每个对象的模型矩阵，并在绘制之前将其传递给着色器
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lightingShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

glm::mat4x4 &RenderLight(const Shader &lightCubeShader, unsigned int lightCubeVAO, const glm::mat4 &projection,
                         const glm::mat4 &view, glm::mat4x4 &model) {// 绘制光源对象
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    glBindVertexArray(lightCubeVAO);
    for (auto &pointLightPosition: pointLightPositions) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::translate(model, pointLightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    return model;
}

void Rendering(GLFWwindow *window, const Shader &lightingShader, const Shader &lightCubeShader, unsigned int cubeVAO,
               unsigned int lightCubeVAO, unsigned int diffuseMap, unsigned int specularMap) {// 渲染
    while (!glfwWindowShouldClose(window)) {
        // 每帧时间逻辑
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        processInput(window);

        // 渲染
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        lightPos.z = sin(glfwGetTime() * 2.0f) * 2.0f + 1.0f;

        glm::vec3 pointLightColors[] = {
                glm::vec3(0.2f, 0.2f, 0.6f),
                glm::vec3(0.3f, 0.3f, 0.7f),
                glm::vec3(0.0f, 0.0f, 0.3f),
                glm::vec3(0.4f, 0.4f, 0.4f),
        };

        // 设置uniforms/drawing对象之前，激活着色器
        ActiveShade(lightingShader);

        DirLight(lightingShader);

        PrtLight(lightingShader, pointLightColors);

        SpotLight(lightingShader);

        // 视图和投影 变换
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // 全局变换
        glm::mat4x4 model;
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // 绑定漫反射贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // 绑定镜面反射贴图
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        RenderObject(lightingShader, cubeVAO);

        model = RenderLight(lightCubeShader, lightCubeVAO, projection, view, model);


        // glfw: 交换缓冲区和检测输入输出（按键/释放、鼠标移动等）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    // glfw的初始化和配置
    GlfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw窗口创建
    GLFWwindow *window = GlfwWindow();

    // 让GLFW捕捉鼠标动作
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad加载所有OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 配置全局的OpenGL状态
    glEnable(GL_DEPTH_TEST);

    // 创建和编译着色器zprogram
    Shader lightingShader("../multiple_lights.vs", "../multiple_lights.fs");
    Shader lightCubeShader("../light_cube.vs", "../light_cube.fs");
    unsigned int VBO;
    unsigned int cubeVAO;

    CubaSetting(VBO, cubeVAO);

    unsigned int lightCubeVAO = LightSetting();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 更新了灯的位置属性的步长，以反映更新的缓冲区数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    unsigned int diffuseMap;
    unsigned int specularMap;

    LoadTextture(diffuseMap, specularMap);

    // 着色器配置
    ShadeSetting(lightingShader);

    Rendering(window, lightingShader, lightCubeShader, cubeVAO, lightCubeVAO, diffuseMap, specularMap);

    // 一旦资源超出其用途，则取消分配：
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // 终止，清除所有先前分配的GLFW
    glfwTerminate();
    return 0;
}