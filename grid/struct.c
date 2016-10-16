#include<stdio.h>
#include<math.h>
typedef float var;
typedef struct vector 
{
        var x;
        var y; 
}vector;
var dotvector(vector x,vector y)
{    
     var sum = x.x*y.x + x.y*y.y;
     return sum;   
}
var absolute(vector x)
{
     var temp;
     temp = sqrt((x.x*x.x)+(x.y*x.y));
     return temp;
}
main()
{
   
    // *******************************scan input
    var xmin,xmax,ymin,ymax;
    int numobject,numbird;
    var objectx[1000];
    var objecty[1000];
    var objectr[1000];
    var objectt[1000];
    int i;
    // size of map
    scanf("%f %f %f %f",&xmin,&xmax,&ymin,&ymax);
    // number of object
    scanf("%d",&numobject);
    // scan object
    for(i=0 ; i < numobject ; i++)
    {
            scanf("%f %f %f %f",&objectx[i],&objecty[i],&objectr[i],&objectt[i]);
    }
    //number of bird
    scanf("%d",&numbird);
    // ********************************end scan input
    
    
    // *****************Start
    
    
    
    
    
    
    
    getchar();
  
}
