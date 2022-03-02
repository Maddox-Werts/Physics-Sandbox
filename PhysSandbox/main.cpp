#include <GLUT/glut.h>
#include <iostream>
#include <vector>

#define BlockSize 30

struct Block{
private:
    float sizeSCL = 1;
    float sidemoves = 0;
    
    bool isStationary = true, onBottom = false;
    
public:
    // Variables
    enum Types {Sand, Water};
    
    float x,y;
    int id;
    Types blockType;
    
    // Constructors
    Block(){
        x=0;y=0;
    }
    Block(float x, float y, int id, Types blockType){
        this->x = x; this->y = y;
        this->id = id; this->blockType=blockType;
    }
    
    // Functions
    void Draw(){
        // just for use
        float sizeHALF = sizeSCL / 2;
        
        // GL Begin
        glBegin(GL_QUADS);
        
        // Making the CUBE
        
        // Left
        switch(blockType){
            case Sand:
                glColor3f(0.5f,0.5f,0);
                break;
            case Water:
                glColor3f(0,0,0.5f);
        }
        glVertex3f((x-sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, sizeHALF/BlockSize);
        glVertex3f((x-sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, -sizeHALF/BlockSize);
        glVertex3f((x-sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, -sizeHALF/BlockSize);
        glVertex3f((x-sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, sizeHALF/BlockSize);
        
        // Left
        glVertex3f((x+sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, -sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, -sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, sizeHALF/BlockSize);
        
        // Top
        glVertex3f((x-sizeHALF)/BlockSize,(y+sizeHALF)/BlockSize,-sizeHALF/BlockSize);
        glVertex3f((x-sizeHALF)/BlockSize,(y+sizeHALF)/BlockSize,sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize,(y+sizeHALF)/BlockSize,sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize,(y+sizeHALF)/BlockSize,-sizeHALF/BlockSize);
        
        // Front
        switch(blockType){
            case Sand:
                glColor3f(0.5f,0.5f,0);
                break;
            case Water:
                glColor4f(0,0,0.5f, 0.5f);
        }
        
        glVertex3f((x-sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, sizeHALF/BlockSize);
        glVertex3f((x+sizeHALF)/BlockSize, (y-sizeHALF)/BlockSize, sizeHALF/BlockSize);
        switch(blockType){
            case Sand:
                glColor3f(1.0f,1.0f,0);
                break;
            case Water:
                glColor4f(0,0,1.0f, 0.5f);
        }
        glVertex3f((x+sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, sizeHALF/BlockSize);
        glVertex3f((x-sizeHALF)/BlockSize, (y+sizeHALF)/BlockSize, sizeHALF/BlockSize);
        
        glEnd();
    }
    
    void Move(std::vector<Block> blocks){
        float sizeHALF = sizeSCL / 2;
        
        bool NotonBottom = (y - 1)/BlockSize > -1;
        onBottom = !NotonBottom;
        
        if(onBottom) {isStationary = true;}
        
        // If we're above the ground
        if(NotonBottom){
            // Getting all sands
            bool canMove = false;
            bool canMoveDLeft = false, canMoveDRight = false;
            bool canMoveLeft=false, canMoveRight=false;
            
            int bhits=0, dlhits=0, drhits=0;
            int rhits=0, lhits=0;
            
            for(int i = 0; i < blocks.size(); i++){
                // For the L&R movement
                // Down Moving Stops
                if(!(blockType == Sand && blocks[i].blockType == Water)){
                    if(y - sizeHALF == blocks[i].y + sizeHALF && x == blocks[i].x){
                        bhits++;
                    }
                    // Can't move down left
                    if(x - sizeHALF == blocks[i].x + sizeHALF && y - sizeHALF == blocks[i].y + sizeHALF){
                        dlhits++;
                    }
                    // Can't move down right
                    if(x + sizeHALF == blocks[i].x - sizeHALF && y - sizeHALF == blocks[i].y + sizeHALF){
                        drhits++;
                    }
                    // Can't move left
                    if(x - sizeHALF == blocks[i].x + sizeHALF && y == blocks[i].y){
                        lhits++;
                    }
                    // Can't move right
                    if(x + sizeHALF == blocks[i].x - sizeHALF && y == blocks[i].y){
                        rhits++;
                    }
                    // Sand in water
                    if(blockType == Water && blocks[i].blockType == Sand){
                        if(x == blocks[i].x && y == blocks[i].y){
                            y += 2;
                            sidemoves = 0;
                        }
                    }
                }
                
                // Move Up Because OVERLAPPING!
                if(x == blocks[i].x && y == blocks[i].y && id != blocks[i].id){
                    //y+=1;
                }
            }
            
            canMove = bhits == 0;
            canMoveDLeft = dlhits == 0;
            canMoveDRight = drhits == 0;
            
            if(blockType == Water){
                canMoveLeft = lhits == 0;
                canMoveRight = rhits == 0;
            }
            
            isStationary = true;
            
            if(canMove){
                y -= 1.f;
                isStationary = false;
                sidemoves = 0;
            }
            else{
                if(canMoveDLeft){
                    y -= 1.f;
                    x -= 1.f;
                    isStationary = false;
                    sidemoves = 0;
                }
                else if(canMoveDRight){
                    y -= 1.f;
                    x += 1.f;
                    isStationary = false;
                    sidemoves = 0;
                }
                
                if(canMoveLeft && -sidemoves <= 0){
                    x -= 1.f;
                    isStationary = false;
                    sidemoves++;
                }
                else if(canMoveRight && -sidemoves <= 0){
                    x += 1.f;
                    isStationary = false;
                    sidemoves++;
                }
            }
            
        }
    }
};

std::vector<Block>blocks;
int selectedItem = 0;

float backgroundRot = 0;

float looky = 0;
float lookx = 0;

void RecieveMouse(int button, int state, int x, int y){
    if(button == 0){
        int blockX, blockY;
        
        blockX = (x - 400) / (BlockSize>>1);
        blockY = (-y + 300) / (BlockSize>>1);
        
        Block::Types bType = Block::Sand;
        
        switch(selectedItem){
            case 0:
                bType = Block::Sand;
                blocks.push_back(Block(blockX, blockY + 2, blocks.size(), Block::Sand ));
                blocks.push_back(Block(blockX, blockY, blocks.size(), Block::Sand ));
                blocks.push_back(Block(blockX, blockY - 2, blocks.size(), Block::Sand ));
                break;
            case 1:
                bType = Block::Water;
                blocks.push_back(Block(blockX, blockY + 2, blocks.size(), Block::Water ));
                break;
        }
        
        std::cout << "Mouse Position = " << blockX << ", " << blockY << "\n";
    }
}

void KeyboardRecieved(unsigned char key, int x, int y){
    std::cout << "Key Pressed: " << key << "\n";
    
    if(key == '1'){
        selectedItem = 0;
    }
    else if(key == '2'){
        selectedItem = 1;
    }
    
    float turnspeed = 0.5f;
    
    if(key == 'w'){
        looky += turnspeed;
    }
    else if(key == 's'){
        looky -= turnspeed;
    }
    else{
        
    }
    
    if(key == 'd'){
        lookx += turnspeed;
    }
    else if(key == 'a'){
        lookx -= turnspeed;
    }
    else{
        
    }
}

void Display(){
    // Clearing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    // new Object
    glLoadIdentity();
    glTranslatef(0, 0, -3.0f);
    
    // Looking
    gluLookAt(0, 0, 0, (GLdouble)lookx, 0, -3.0f, 0, 1, 0);
    
    // Drawing sand
    for(int i = 0; i < blocks.size(); i++){
        blocks[i].Draw();
    }
    
    // Drawing the used item text
    std::string itemText;
    switch(selectedItem){
        case 0:
            itemText = "Sand";
            break;
        case 1:
            itemText = "Water";
    }
    
    glColor3f(1,1,1);
    glRasterPos2f(-0.75f, 0.75f);
    
    for(int i = 0; i < itemText.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, itemText[i]);
    }
    
    glLoadIdentity();
    
    // Looking
    gluLookAt(0, 0, 0, (GLdouble)lookx, 0, -6.0f, 0, 1, 0);
    
    glTranslatef(0.0f, -0.2f, -3.0f);
    glRotatef(backgroundRot, 0, 1, 0);
    
    glBegin(GL_TRIANGLES);
    
    float size = 0.05f;
    
    // Front
    glColor3f(1,1,0);
    glVertex3f(0,size,0);
    
    glColor3f(0,0,0);
    glVertex3f(size,-size,size);
    
    glColor3f(0,0,0);
    glVertex3f(-size,-size,size);
    
    // Left
    glColor3f(1,1,0);
    glVertex3f(0,size,0);
    
    glColor3f(0,0,0);
    glVertex3f(-size,-size,-size);
    
    glColor3f(0,0,0);
    glVertex3f(-size,-size,size);
    
    // Back
    glColor3f(1,1,0);
    glVertex3f(0,size,0);
    
    glColor3f(0,0,0);
    glVertex3f(size,-size,-size);
    
    glColor3f(0,0,0);
    glVertex3f(-size,-size,-size);
    
    // Right
    glColor3f(1,1,0);
    glVertex3f(0,size,0);
    
    glColor3f(0,0,0);
    glVertex3f(size,-size,-size);
    
    glColor3f(0,0,0);
    glVertex3f(size,-size,size);
    
    glEnd();
    
    // Passing
    glutSwapBuffers();
}

void ReShape(GLsizei width, GLsizei height){
    // Fixing thing
    if(height == 0) height = 1;
    // Making Aspect
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    // Setting the viewport
    glViewport(0, 0, width, height);
    // Matrixing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perception
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
    
    // Code here
    for(int i = 0; i < blocks.size(); i++){
        blocks[i].Move(blocks);
    }
    
    // Background
    backgroundRot += 0.8f;
    
    if(backgroundRot > 360) backgroundRot -= 360;
    if(backgroundRot < 0  ) backgroundRot += 360;
}

void Start(){
    // GL Init
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glShadeModel(GL_SMOOTH);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

// Application
int main(int argc, char** argv){
    
    // Initilizing GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    
    // Window
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Physics Sandbox");
    
    // Making extras
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutMouseFunc(RecieveMouse);
    glutKeyboardFunc(KeyboardRecieved);
    glutTimerFunc(0, timer, 0);
    
    Start();
    
    // Game Loop
    glutMainLoop();
    
    // Exit
    return 0;
}
