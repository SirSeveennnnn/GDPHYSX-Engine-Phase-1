#version 330 core //Version

out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red
void main()
{
    //               r   g   b   a     Ranges from 0->1
    FragColor = vec4(1, 1, 1, 1f); //Sets the color of the Fragment
}