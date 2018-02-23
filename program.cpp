//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>

/*
	0: normal
	1: text
*/
/*void changeMouse(int mode);
void print(const char* n);
void floatprint(float n);
void print(int n);
int rand();
float sin(float x);
float cos(float x);
int ceil(float x);
float abs(float x) { return x < 0 ? -x : x; };*/
int rand();

// width: 320, height: 240

unsigned char screen[320 * 240 * 3];

const int w = 320;
const int h = 240;


const float math_pi = 3.14159265359f;

static inline float math_sin( float x ) {
    return sinf( x );
}

static inline float math_cos( float x ) {
    return cosf( x );
}

// the texture map
int texmap[ 16 * 16 * 16 * 3 ];

// the voxel map
char map[ 64 * 64 * 64 ];

static inline int random( int max ) {
    return (rand()^(rand()<<16)) % max;
}

static void makeTextures( void ) {
    
    // each texture
    for ( int j=0; j<16; j++ ) {

        int k = 255 - random(96);

        // each pixel in the texture
        for ( int m=0; m<16 * 3; m++ )
            for ( int n = 0; n<16; n++ ) {

                int i1 = 0x966C4A;
                int i2 = 0;
                int i3 = 0;

                if (j == 4)
                    i1 = 0x7F7F7F;
                if ((j != 4) || (random(3) == 0))
                    k = 255 - random(96);
                if ( j == 1 )
                {
                    if (m < (((n * n * 3 + n * 81) >> 2) & 0x3) + 18)
                        i1 = 0x6AAA40;
                    else if (m < (((n * n * 3 + n * 81) >> 2) & 0x3) + 19)
                        k = k * 2 / 3;
                }
                if (j == 7)
                {
                    i1 = 0x675231;
                    if ((n > 0) && (n < 15) && (((m > 0) && (m < 15)) || ((m > 32) && (m < 47))))
                    {
                        i1 = 0xBC9862;
                        i2 = n - 7;
                        i3 = (m & 0xF) - 7;
                        if (i2 < 0)
                            i2 = 1 - i2;

                        if (i3 < 0)
                            i3 = 1 - i3;

                        if (i3 > i2)
                            i2 = i3;

                        k = 196 - random(32) + i2 % 3 * 32;
                    }
                    else if (random(2) == 0)
                        k = k * (150 - (n & 0x1) * 100) / 100;
                }
                if (j == 5)
                {
                    i1 = 0xB53A15;
                    if (((n + m / 4 * 4) % 8 == 0) || (m % 4 == 0))
                        i1 = 0xBCAFA5;
                }
                i2 = k;
                if (m >= 32)
                    i2 /= 2;
                if (j == 8)
                {
                    i1 = 5298487;
                    if (random(2) == 0)
                    {
                        i1 = 0;
                        i2 = 255;
                    }
                }

                // fixed point colour multiply between i1 and i2
                i3 =
                    ((((i1 >> 16) & 0xFF) * i2 / 255) << 16) |
                    ((((i1 >>  8) & 0xFF) * i2 / 255) <<  8) |
                      ((i1        & 0xFF) * i2 / 255);
                // pack the colour away
                texmap[ n + m * 16 + j * 256 * 3 ] = i3;
            }
    }
}

static void makeMap( void ) {
    // add random blocks to the map
	for ( int x = 0; x < 64; x++) {
		for ( int y = 0; y < 64; y++) {
			for ( int z = 0; z < 64; z++) {
				int i = (z << 12) | (y << 6) | x;
				float yd = ((float)y - 32.5f) * 0.4f;
				float zd = ((float)z - 32.5f) * 0.4f;
				map[i] = random( 16 );

                float th = random( 256 ) / 256.0f;

				if (th > sqrtf( sqrtf( (yd * yd) + (zd * zd) ) ) - 0.8f)
					map[i] = 0;
			}
		}
	}
}

static void init( void ) {
    makeTextures( );
    makeMap( );
}

// fixed point byte byte multiply
static inline int fxmul( int a, int b ) {
    return (a*b)>>8;
}

int positionTime = 0;

static void render( void ) {


    float now = (float)(positionTime % 10000) / 10000.f;
	positionTime += 32;

	float xRot = math_sin(now * math_pi * 2.0f) * 0.4f + math_pi / 2.0f;
	float yRot = math_cos(now * math_pi * 2.0f) * 0.4f;
	float yCos = math_cos(yRot);
	float ySin = math_sin(yRot);
	float xCos = math_cos(xRot);
	float xSin = math_sin(xRot);

	float ox = 32.5f + now * 64.0f;
	float oy = 32.5f;
	float oz = 32.5f;

	// for each column
	for ( int x = 0; x < w; x++) {
		// get the x axis delta
		float ___xd = ((float)x - (float)w / 2.f) / (float)h;
		// for each row
		for ( int y = 0; y < h; y++) {
			// get the y axis delta
			float  __yd = ((float)y - (float)h / 2.f) / (float)h;
			float  __zd = 1.0f;
			float ___zd =  __zd * yCos +  __yd * ySin;
			float   _yd =  __yd * yCos -  __zd * ySin;
			float   _xd = ___xd * xCos + ___zd * xSin;
			float   _zd = ___zd * xCos - ___xd * xSin;

			int col = 0;
			int br = 255;
			float ddist = 0.0f;

			float closest = 32.f;
			
			// for each principle axis  x,y,z
			for ( int d = 0; d < 3; d++) {
				float dimLength = _xd;
				if (d == 1)
					dimLength = _yd;
				if (d == 2)
					dimLength = _zd;

				float ll = 1.0f / (dimLength < 0.0f ? -dimLength : dimLength);
				float xd = (_xd) * ll;
				float yd = (_yd) * ll;
				float zd = (_zd) * ll;

				float       initial = ox - floor(ox);
				if (d == 1) initial = oy - floor(oy);
				if (d == 2) initial = oz - floor(oz);

				if (dimLength > 0) initial = 1 - initial;

				float dist = ll * initial;

				float xp = ox + xd * initial;
				float yp = oy + yd * initial;
				float zp = oz + zd * initial;

				if (dimLength < 0) {
					if (d == 0)	xp--;
					if (d == 1)	yp--;
					if (d == 2)	zp--;
				}

				// while we are concidering a ray that is still closer then the best so far
				while (dist < closest) {
				
					// quantize to the map grid
					int tex = map[ (((int)zp & 63) << 12) | (((int)yp & 63) << 6) | ((int)xp & 63) ];

					// if this voxel has a texture applied
					if (tex > 0) {

						// find the uv coordinates of the intersection point
						int u = ((int)((xp + zp) * 16.f)) & 15;
						int v = ((int) (yp       * 16.f)  & 15) + 16;
						
						// fix uvs for alternate directions?
						if (d == 1) {
							u =  ((int)(xp * 16.f)) & 15;
							v = (((int)(zp * 16.f)) & 15);
							if (yd < 0)
								v += 32;
						}
						
						// find the colour at the intersection point
						int cc = texmap[ u + v * 16 + tex * 256 * 3 ];
						
						// if the colour is not transparent
						if (cc > 0) {
							col = cc;
							ddist = 255.0f - ((dist / 32.0f * 255));
							br = 255.0f * (255 - ((d + 2) % 3) * 50.0f) / 255.0f;
							
							// we now have the closest hit point (also terminates this ray)
							closest = dist;
						}
					}
					
					// advance the ray
					xp += xd;
					yp += yd;
					zp += zd;
					dist += ll;
				}
			}
			
			int b = fxmul( br, ddist );
			
			int _r = (((col>>16) & 0xff) * b) >> 8;
			int _g = (((col>> 8) & 0xff) * b) >> 8;
			int _b = (((col    ) & 0xff) * b) >> 8;

			screen[(y * w + x) * 3 + 0] = _r;
			screen[(y * w + x) * 3 + 0] = _g;
			screen[(y * w + x) * 3 + 0] = _b;
        }
    }
}


int main() {
	init();
    return 0;
}

bool changes = false;

void timer() {
    render();
}