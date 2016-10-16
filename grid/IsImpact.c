typedef struct vector
{
        var x;
        var y;
        float sx;
        float sy;
}vector;
typedef struct cicle
{
        var x;
        var y;
        var r;
}circle;
float IsImpact(vector u,circle c)
{
        float b;
        float size;
        vector v;
        // หา unit vector
        // หาขนาดของ vector
        size = absolute(u);
        u.x/=size;
        u.y/=size;
        // หา vector ตัวที่สอง
        v.x = c.x - u.sx;
        v.y = c.y - u.sy;
        v.sx = u.sx;
        v.sy = u.sy;

        size = dotvector(v,u);

        b = sqrt( pow(absolute(v),2)-pow(size,2) );
        if ( b < c.r )
                return b;
        else
                return 0
}

