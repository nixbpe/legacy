#include<stdio.h>
#include<math.h>
#include <sys/time.h>
#define round 1
struct timeval tp_start;

typedef double var;
typedef struct vector
{
        double x;
	double y;
        double sx;
        double sy;
}vector;

typedef struct circle
{
        double x;
        double y;
        double r;
		int  type;
}circle;

typedef struct point
{
    var x;
    var y;

}point;

typedef struct area{
	var x1,x2,y1,y2;
}area;

void random_start(double *x, double *y,
		  double *dx, double *dy,
		  double min_x, double max_x,
		  double min_y, double max_y)
{
  int dir = (rand()>>2) % 4;
 
  int dir_array[][2] = { {0,-1}, {-1,0}, {0,1}, {1,0} };
 
  if((dir==0) || (dir==2))
    *x = min_x + rand() % (int)(max_x - min_x + 1);
  else {
    if(dir==1)
      *x = max_x;
    else
      *x = min_x;
  }
 
  if((dir==1) || (dir==3))
    *y = min_y + rand() % (int)(max_y - min_y + 1);
  else {
    if(dir==0)
      *y = max_y;
    else
      *y = min_y;
  }
  
  *dx = dir_array[dir][0];
  *dy = dir_array[dir][1];

	/**x = 1;
	*y = 0;
	*dx = -30;
	*dy = 2;*/
}

// ใ้ชตรวจสอบว่า vector ัตัดวงกลม ?
double IsImpact(vector u,circle c);
var Dotvector(vector x,vector y);
var Absolute(vector x);
vector Unitvector(vector x);
// หาจุดตัดของ vector กับ วงกลม
vector Findjudtud(circle x,vector chon,double b); 
vector Plus(vector x,vector y);
vector Minus(vector x,vector y);
// หา vector สะท้อน
vector Findnew(vector in,circle cc);

int main( int argc, char* argv[] )
{
 	freopen(argv[1],"r",stdin);
	circle op[1000];
	circle temp[1000];
	area tb;
	vector in;
	vector tmp,tmp2;
	vector tmpans[10];
	vector ans[10];
	int impact = -1,ti=-1;
	int count = 0;
	int n=10;
	int i,j,tua;
	double b=-1;
	double tmp_b;
	double min = 9999;
	double xmin,xmax,ymin,ymax;
	double ix,iy,isx,isy;
	int sumscore=0;
	int score = 0;
	int nbird = 1;
    	int max = -1;
	//printf("%s",argv[1]);
	gettimeofday(&tp_start,NULL);
	srand(tp_start.tv_sec);
	scanf("%lf %lf %lf %lf",&xmin,&xmax,&ymax,&ymin);
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%lf %lf %lf %d",&temp[i].x,&temp[i].y,&temp[i].r,&temp[i].type);
	}
	scanf("%d",&nbird);
	score = 0;
	for(j=0 ; j < round ;j++)
	{ 
		 for(i=0 ; i<n ;i++)
			 {
				     op[i].x = temp[i].x;
				     op[i].y = temp[i].y;
				     op[i].r = temp[i].r;
				     op[i].type = temp[i].type;
			 }
		sumscore = 0;
		for(tua = 0 ; tua < nbird ; tua++)
	    	{
		     	//printf("round : %d,time : %d\n",j,time);
			 b=-1;
			 tmp_b;
			 min = 9999;
			 score = 0;
			 count = 0;
	    		random_start(&isx,  &isy, &ix,  &iy, xmin,  xmax, ymin,  ymax);
			//===========================================  debug
			//isx = -9.000000;
			//isy = 200.000000;
			//ix = 0.000000; 
			//iy = -1.000000;
			//============================================
	    		in.x = ix;
	    		in.y = iy;
	    		in.sx = isx;
	    		in.sy = isy;
	   		tmpans[tua].x = ix;
	    		tmpans[tua].y = iy;
	    		tmpans[tua].sx = isx;
	    		tmpans[tua].sy = isy;
	    		while(1)
	    		{
				printf("%.5lf,%.5lf\n",in.sx,in.sy);
	    			b = 0;
	    			impact = -1;
	    			min = 9999;
	    			for(i=0;i<n;i++)
	    			{
	    				//printf("%d\n",op[i].type);
	    				if(op[i].type == 0)
	    				{
	    					b = IsImpact(in,op[i]);
	    					//printf("%d %lf \n ",i,b);
	    					if(b>-1 && i!=ti)
	    					{
	    						tmp = Findjudtud(op[i],in,b);
	    						if(Absolute(tmp) < min && Absolute(tmp)>=0.000001)
	    						{
	    							min = Absolute(tmp);
	    							tmp2 = tmp;
	    							impact = i;
	    						}
	    					}
	    				}
	    			}
	    			if(impact != -1 && count <= 100)
	    			{
					if(count == 0)
					{
						for(i=0;i<n;i++)
		    				{
		    					b=IsImpact(tmp2,op[i]);
		    					tmp = Findjudtud(op[i],tmp2,b);
		    					if(b>=0 && Absolute(tmp) < Absolute(tmp2) )
		    					{ 
		    						if(op[i].type==1)
		    							score+=10;
		    						if(op[i].type==2)
		    							score+=1000;
		    						if(op[i].type == 1 || op[i].type==2)
		    							op[i].type = -1; 
		    					}				
		    				}
						//printf("!!Fist line score :%d\n",score);
					}
					score++;
	    				count++;
	    				ti=impact;
	    				in = Findnew(tmp2,op[impact]);
	    				for(i=0;i<n;i++)
	    				{
	    					b=IsImpact(in,op[i]);
	    					tmp = Findjudtud(op[i],in,b);
	    					if(b>=0 && Absolute(tmp) < Absolute(in) )
	    					{ 
	    						if(op[i].type==1)
	    							score+=10;
	    						if(op[i].type==2)
	    							score+=1000;
	    						if(op[i].type == 1 || op[i].type==2)
	    							op[i].type = -1; 
	    					}				
	    				}
	    			}
	    			else
				{
					/*if(count!=100)
						for(i=0;i<n;i++)
					    	{
					    		b=IsImpact(in,op[i]);
					    		if(b>-1)
					    		{ 
					    			if(op[i].type==1)
					    				score+=10;
					    			if(op[i].type==2)
					    				score+=1000;
					    			if(op[i].type == 1 || op[i].type==2)
					    				op[i].type = -1; 
					    		}				
					    	}*/
	    				break;
				}
	    			//printf("!!score :%d\n",score);
	    		} // job 1 tua
			printf("==\n");
			sumscore += score;
			printf("!!!%d\n",sumscore);
        	}// job 1 round
		if(sumscore > max)
			{
				max = sumscore;
				for(i=0 ; i < nbird ;i++)
				{ 
				 	ans[i].x = tmpans[i].x;
				 	ans[i].y = tmpans[i].y;
				 	ans[i].sx = tmpans[i].sx;
					ans[i].sy = tmpans[i].sy;
				}
			}
		printf("round : %d,score :%d\n",j,max);
		// ให้ vector วิ่งให้ตกขอบ
		in.x*=1000;
		in.y*=1000;
		in.sx+=in.x;
		in.sy+=in.y;
		//printf("%.5lf,%.5lf",in.sx,in.sy);
		
	}//job all round
	for(i=0 ; i < nbird ;i++)
			printf("%lf %lf %lf %lf\n",ans[i].sx,ans[i].sy,ans[i].x,ans[i].y);
	printf("sum: %d\n",max);
}

double IsImpact(vector u,circle c)
{
        double b;
        double size;
        vector v;
        // หา unit vector
        // หาขนาดของ vector
        size = Absolute(u);
        u.x/=size;
        u.y/=size;
        // หา vector ตัวที่สอง
        v.x = c.x - u.sx;
        v.y = c.y - u.sy;
        v.sy = u.sy;
	v.sx = u.sx;
        size = Dotvector(v,u);

        b = sqrt( pow(Absolute(v),2)-pow(size,2) );
        if ( b < c.r && size > 0 && (c.r-b)>0.000001)
                return b;
        else
                return -1;
}


var Dotvector(vector x,vector y)
{    
     var sum = x.x*y.x + x.y*y.y;
     return sum;   
}

var Absolute(vector x)
{
     var temp;
     temp = sqrt(      (x.x)*(x.x)  +   (x.y)*(x.y)  );
     return temp;
}

vector Unitvector(vector x)
{
    double length=Absolute(x);
    x.x=x.x/length;
    x.y=x.y/length;
    return x;
}

vector Findjudtud(circle x,vector chon,double b)
{
    double len;
    double a;
    point tmp;
    vector v;
    v.x=x.x-chon.sx;
    v.y=x.y-chon.sy;
    len=Dotvector(v,Unitvector(chon));
    a= sqrt((x.r*x.r)-(b*b));
    len=len-a;
    chon=Unitvector(chon);
    chon.x=chon.x*len;
    chon.y=chon.y*len; 
    return chon;
}

vector Plus(vector x,vector y)
{
       vector res;
       res.x = x.x+y.x;
       res.y = x.y+y.y;
       res.sx = 0;
       res.sy = 0;
       return res;
}

vector Minus(vector x,vector y)
{
       vector res;
       res.x = x.x-y.x;
       res.y = x.y-y.y;
       res.sx = 0;
       res.sy = 0;
       return res;
}

vector Findnew(vector in,circle cc)
{
       vector u;
       vector temp;
       point judtud;
       judtud.x=in.x+in.sx;
       judtud.y=in.y+in.sy;  
       u.x = -(cc.y-judtud.y); //- y
       u.y = cc.x-judtud.x;  // x
       u.sx = 0;
       u.sy = 0;
       double angle = acos(Dotvector(u,in)/(Absolute(in)*Absolute(u)));
	//printf("angle : %lf\n",angle);
       if(angle > 22.0/14.0)
       {
                u.x = cc.y-judtud.y;
                u.y = -(cc.x-judtud.x);     
       }
       u = Unitvector(u);
       in.x = -in.x;
       in.y = -in.y;
       temp.x = 2*Dotvector(in,u);
       temp.y = 2*Dotvector(in,u);
       temp.sx = 0;
       temp.sy = 0;
       u.x = u.x*temp.x;
       u.y = u.y*temp.y;
       temp = Minus(in,u);
       temp.sx = judtud.x;
       temp.sy = judtud.y;
       return temp;
}
