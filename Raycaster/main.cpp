#include <iostream>
#include <GL/glut.h>
#include <cmath>
#define PI 3.1315926535897932384626

float px, py, pa;
int pxR, pyR;
int player_looking_at;
void draw_player()
{
    glColor3f(1, 1, 1);
    glPointSize(8);

    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}

int map[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},

};

int mapI = 8, mapJ = 8;

void draw_map()
{
    for (int ii = 0; ii < mapI; ii++)
    {
        for (int ij = 0; ij < mapJ; ij++)
        {
            if (map[ii][ij] == 1)
            {
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);

                int posI = ii * 64;
                int posJ = ij * 64;

                glVertex2i(posJ, posI);
                glVertex2i(posJ + 64, posI);
                glVertex2i(posJ + 64, posI + 64);
                glVertex2i(posJ, posI + 64);
                glEnd();
            }
        }
    }
}

void draw_perpendicular_lines()
{
    for (int i = 0; i < 8; i++)
    {
        glColor3f(0, 1, 0);
        glLineWidth(1);
        glBegin(GL_LINES); // horizontal lines
        glVertex2i(i * 64, 0);
        glVertex2i(i * 64, 512);
        glEnd();

        glBegin(GL_LINES); // vertical lines
        glVertex2i(0, i * 64);
        glVertex2i(512, i * 64);
        glEnd();
    }
}

void draw_direction_line()
{
    glColor3f(1, 1, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px - cos(pa) * 30, py - sin(pa) * 30);
    glEnd();
}

/* for(int ii = 0; ii < mapI; ii++){
            for (int ij=0; ij < mapJ; ij++){

            }
        }
 */

int round64(int x) // rounds to nearest multiple of 64
{
    int rounded, next_rounded, big_remainder;
    rounded = x - (x % 64);
    next_rounded = rounded + 64;
    big_remainder = next_rounded - x;

    if (big_remainder < (x % 64))
    {
        return next_rounded;
    }
    else
    {
        return rounded;
    }
}

int round64_near(int x) // rounds to nearest multiple of 64
{
    int rounded, next_rounded, big_remainder;
    rounded = x - (x % 64);

    return rounded;
}

int round64_far(int x) // rounds to nearest multiple of 64
{
    int rounded, next_rounded, big_remainder;
    rounded = x - (x % 64);
    next_rounded = rounded + 64;

    return next_rounded;
}

int smaller(int x, int y) // returns 1 if the first number is smaller, 2 if the second, returns 0 if equal
{

    if (y < x)
    {
        return 2;
    }
    else if (x < y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int lengths[128];
char walls[128];
bool keystates[20]; // 0 = w, 1 = a, 2 = s, 3 = d

void draw_raycast()
{
    for (int i = 0; i < 128; i++)
    {
        int ni = 512;
        int ni2 = 512;
        int rx, ry;   // ni number of iterations left
        int pxs, pys; // int playerx and playery simplified
        float ra, ra2, dx;
        int oppS; // trinometry shenanigans
        int iterator = 1;
        int iterator2 = 1;

        int lengthX, lengthY;

        pxs = (int)(px / 64);
        pys = (int)(py / 64);
        if (player_looking_at > 32 && player_looking_at <= 63) // checks if player is facing left
        {

            ra = pa - (PI / 4) + (PI / 256) * i;
            ra2 = pa - (PI / 4) + (PI / 256) * i;
        }
        else if (player_looking_at > 0 && player_looking_at < 32) // checks if player is looking right
        {
            ra = pa - (PI / 4) + (PI / 256) * i;
            ra2 = pa - (PI / 4) + (PI / 256) * i;
        }
        else if (player_looking_at == 0)
        {
            ra = PI / 2 - (PI / 4) + (PI / 256) * i;
            ra2 = PI / 2 - (PI / 4) + (PI / 256) * i;
        }
        else if (player_looking_at == 32)
        {
            ra = 3 * (PI / 2) - (PI / 4) + (PI / 256) * i;
            ra2 = 3 * (PI / 2) - (PI / 4) + (PI / 256) * i;
        }
        else if (player_looking_at == 16)
        {
            ra = 2 * (PI) - (PI / 4) + (PI / 256) * i;
            ra2 = 2 * (PI) - (PI / 4) + (PI / 256) * i;
        }
        else if (player_looking_at == 48)
        {
            ra = PI - (PI / 4) + (PI / 256) * i;
            ra2 = PI - (PI / 4) + (PI / 256) * i;
        }

        rx = pxs; // you have to find a way to make the hypotenuse 64

        ry = pys; // you have to find a way to make the hypotenuse 64

        bool checkingX = true;
        bool checkingY = true;

        while (ni > 0)
        {
            // float hypI = 64 * sqrt(2);

            float hypI = 1;

            hypI = hypI * iterator;

            int opp = sin(ra) * hypI;
            int oppS = opp / 64;

            int adj = cos(ra) * hypI;
            int adjS = adj / 64;

            int rxs = (px - adj) / 64;
            int rys = (py - opp) / 64;

            if (map[rys][rxs] == 1 && checkingX)
            {

                checkingX = false;
                lengthX = px - (px - adj);

                /*   if (lengthX < 0)
                  {
                      lengths[i] = -lengthX;
                  }
                  else
                  {
                      lengths[i] = lengthX;
                  } */
            }

            ni--;
            iterator++;
        }

        while (ni2 > 0)
        {
            // float hypI = 64 * sqrt(2);
            float hypI2 = 1;

            hypI2 = hypI2 * iterator2;

            int opp2 = sin(ra2) * hypI2;
            int oppS2 = opp2 / 64;

            int adj2 = cos(ra2) * hypI2;
            int adjS2 = adj2 / 64;

            int rxs2 = (px - adj2) / 64;
            int rys2 = (py - opp2) / 64;

            if (map[rys2][rxs2] == 1 && checkingY)
            {
                checkingY = false;
                lengthY = py - (py - opp2);

                /* if (lengthY < 0)
                {
                    lengths[i] = -lengthY;
                }
                else
                {
                    lengths[i] = lengthY;
                } */
            }

            ni2--;
            iterator2++;
        }

        glColor3f(1, 1, 0);
        glPointSize(6);
        glBegin(GL_POINTS);
        glVertex2i(px - lengthX, py - lengthY);
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(px - lengthX, py - lengthY);
        glEnd();

        int nlengthX, nlengthY;
        nlengthX = pow(lengthX,2);
        nlengthY = pow(lengthY,2);
        // if (lengthX < 0){
        //     nlengthX = -nlengthX;
        // }
        // if (lengthY < 0){
        //     nlengthY = -nlengthY;
        // }


        lengths[i] = sqrt(nlengthX + nlengthY);

        // if ((int)(px - lengths[i]) % 64 == 0)
        // {
        //     walls[i] = 'x';
        // }
        // else if ((int)(py - lengths[i]) % 64 == 0)
        // {
        //     walls[i] = 'y';
        // }

        // std::cout << px - lengthX << ',' << py - lengthY << std::endl;
    }
}

void projection()
{
    // if (player_looking_at >= 0 && player_looking_at <= 32)
    {

        for (int i = 0; i < 128; i++)
        {

            int distance = (lengths[i]);
            if (distance <= 0)
            {
                distance = 1;
            }

            if (lengths[i] != lengths[i + 1] && lengths[i] != lengths[i - 1])
            {
                glColor3f(0.5, 0, 1);
            }
            else
            {
                glColor3f(0.7, 0, 1);
            }
            glLineWidth(4);
            glBegin(GL_LINES);
            glVertex2i(512 + i * 4, 256 - (256 * 64) / distance);
            glVertex2i(512 + i * 4, 256 + (256 * 64) / distance);
            glEnd();

            glColor3f(0, 0, 0.4);
            glBegin(GL_LINES);
            glVertex2i(512 + i * 4, 512);
            glVertex2i(512 + i * 4, 256 + (256 * 64) / distance);
            glEnd();
        }
    }
    // else
    // {
    //     for (int i = 128; i > 0; i--)
    //     {
    //         int k = 128 - i;

    //         int distance = (lengths[k]);
    //         if (distance <= 0)
    //         {
    //             distance = 1;
    //         }
    //         if (lengths[k] != lengths[k + 1] && lengths[k] != lengths[k - 1])
    //         {
    //             glColor3f(0.5, 0, 1);
    //         }
    //         else
    //         {
    //             glColor3f(0.7, 0, 1);
    //         }
    //         glLineWidth(4);
    //         glBegin(GL_LINES);
    //         glVertex2i(512 + i * 4, 256 - (256 * 64) / distance);
    //         glVertex2i(512 + i * 4, 256 + (256 * 64) / distance);
    //         glEnd();

    //         glColor3f(0, 0, 0.4);
    //         glBegin(GL_LINES);
    //         glVertex2i(512 + i * 4, 512);
    //         glVertex2i(512 + i * 4, 256 + (256 * 64) / distance);
    //         glEnd();
    //         k++;
    //     }
    // }
}

int timerA = 0, timerD = 0;
void draw()
{

    glClear(GL_COLOR_BUFFER_BIT);
    draw_map();
    draw_perpendicular_lines();
    draw_player();
    draw_direction_line();
    draw_raycast();

    if (keystates[0])
    {
        int pxN = (px - cos(pa) * 2);
        int pyN = (py - sin(pa) * 2);
        if (map[pyN / 64][pxN / 64] == 0)
        {
            px -= cos(pa) * 2;
            py -= sin(pa) * 2;
        }
        glutPostRedisplay();
    }
    else if (keystates[2])
    {
        int pxN = (px + cos(pa) * 2);
        int pyN = (py + sin(pa) * 2);
        if (map[pyN / 64][pxN / 64] == 0)
        {
            px += cos(pa) * 2;
            py += sin(pa) * 2;
        }
        glutPostRedisplay();
    }

    if (keystates[1])
    {
        
        if (timerA == 0)
        {
            pa -= PI / 32;
            if (pa < 0)
            {
                pa += PI * 2;
            }
            player_looking_at -= 1;
            if (player_looking_at < 0)
            {
                player_looking_at += 64;
            }
            glutPostRedisplay();
            timerA = 1;
        }
        else{
        timerA--;
        glutPostRedisplay();
        }
    }
    else if (keystates[3])
    {
        if (timerD == 0){
        pa += PI / 32;

        if (pa > PI * 2)
        {
            pa -= PI * 2;
        }
        player_looking_at += 1;
        if (player_looking_at >= 64)
        {
            player_looking_at -= 64;
        };
        glutPostRedisplay();
        timerD = 1;
        }
        else{
            timerD--;
            glutPostRedisplay();
        }

    }

    projection();
    glutSwapBuffers();
}

// float px, py, pa

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }

    if (key == 'j'){
         pa -= PI / 32;
            if (pa < 0)
            {
                pa += PI * 2;
            }
            player_looking_at -= 1;
            if (player_looking_at < 0)
            {
                player_looking_at += 64;
            }
            glutPostRedisplay();
    }
    if (key == 'k'){
        pa += PI / 32;

        if (pa > PI * 2)
        {
            pa -= PI * 2;
        }
        player_looking_at += 1;
        if (player_looking_at >= 64)
        {
            player_looking_at -= 64;
        };
        glutPostRedisplay();
    }

    if (key == 'r')
    {
        for (int i = 0; i < 128; i++)
        {
            std::cout << i << ':' << lengths[i] << ' ' << walls[i] << std::endl;
        }
    }

    if (key == 'w')
    {
        keystates[0] = true;
    }
    if (key == 's')
    {
        keystates[2] = true;
    }

    if (key == 'a')
    {
        keystates[1] = true;
    }

    if (key == 'd')
    {
        keystates[3] = true;
    }
    if (key == 'p')
    {
        std::cout << px << ',' << py << std::endl
                  << (int)px / 64 << ',' << (int)py / 64 << std::endl
                  << player_looking_at << std::endl;
    }

    glutPostRedisplay();
}

void keyboardup(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        keystates[0] = false;
    }
    if (key == 's')
    {
        keystates[2] = false;
    }

    if (key == 'a')
    {
        keystates[1] = false;
    }

    if (key == 'd')
    {
        keystates[3] = false;
    }

    glutPostRedisplay();
}

void init()
{
    glClearColor(0, 0, 0.1, 0);
    gluOrtho2D(0, 1024, 512, 0);

    px = 270;
    py = 275;
    player_looking_at = 0; // 0
    pa = PI / 2;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("Raycaster");
    glutIgnoreKeyRepeat(GLUT_DEVICE_IGNORE_KEY_REPEAT);

    init();
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);

    glutMainLoop();
}
