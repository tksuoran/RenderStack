#ifndef triangle_hpp_renderstack_geometry
#    define triangle_hpp_renderstack_geometry

#    include "renderstack_geometry/geometry.hpp"
#    include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

class polyhedron : public geometry
{
public:
    polyhedron(const std::string &name)
        m_name(name)
    {
    }

    /*
#declare points=array[1000];
#declare tpoints=array[1000];
#declare npoints=0;
#declare faces=array[1000];
#declare nfaces=0;
*/
    void add_point(double x, double y, double z)
    {
        make_point(x, y, z);
    }

    void add_even_permutations(double x, double y, double z)
    {
        add_point(x, y, z);
        add_point(y, z, x);
        add_point(z, x, y);
    }

    void add_permutations(double x, double y, double z)
    {
        add_even_permutations(x, y, z);
        add_even_permutations(x, z, y);
    }

    void add_points_sign(double x, double y, double z, double sx, double sy, double sz)
    {
        add_point(x, y, z);
        if (sx)
            add_points_sign(-x, y, z, 0.0, sy, sz);
        if (sy)
            add_points_sign(x, -y, z, 0.0, 0.0, sz);
        if (sz)
            add_point(x, y, -z);
    }

    void add_even_permutations_sign(double x, double y, double z, double sx, double sy, double sz)
    {
        add_points_sign(x, y, z, sx, sy, sz)
            add_points_sign(y, z, x >, )
        //add_points_sign(<a.z,a.x,a.y>,)
#    end
#    macro addpermssgn(a, s)
                addevenpermssgn(a, s)
                    addevenpermssgn(<a.x, a.z, a.y>, )
#    end
#    macro addpointsevensgn(a)
                        addpoint(a)
                            addpoint(a * <-1, -1, 1>)
                                addpoint(a * <-1, 1, -1>)
                                    addpoint(a * <1, -1, -1>)
#    end
#    macro addevenpermsevensgn(a)
                                        addevenperms(a)
                                            addevenperms(a * <-1, -1, 1>)
                                                addevenperms(a * <-1, 1, -1>)
                                                    addevenperms(a * <1, -1, -1>)
#    end
#    macro addpermsaltsgn(a)
                                                        addevenpermsevensgn(a)
                                                            addevenpermsevensgn(<a.x, a.z, -a.y>)
#    end
/*#macro addevenpermssgn(a,s) //Calls addevenperms with, for each 1 in s, a.{x,y,z} replaced with {+,-}a.{x,y,z}
  addevenperms(a)
  #if(s.x) addevenpermssgn(a*<-1,1,1>,s*<0,1,1>) #end
  #if(s.y) addevenpermssgn(a*<1,-1,1>,s*<0,0,1>) #end
  #if(s.z) addevenperms(a*<1,1,-1>) #end
#end*/
#    macro addface(d, l)
#    local a                                           = vnormalize(d) / l;
#    local f                                           = 1;
#    local n                                           = 0; #    while (n < nfaces - .5)
#    if (vlength(faces[n] - a) < 0.00001) #    local f = 0; #    end
#        local n = n + 1; #        end
#        if (f)
#            declare faces[nfaces] = a;
#            declare nfaces        = nfaces + 1;
#            end
#            end
#            macro dual()
#            declare temp    = faces;
#            declare faces   = points;
#            declare points  = temp;
#            declare temp    = nfaces;
#            declare nfaces  = npoints;
#            declare npoints = temp;
#            end

#            macro autoface() //WARNING: ONLY WORKS IF ALL EDGES HAVE EQUAL LENGTH
//Find edge length
#            declare elength = 1000;
#            local a = 0; #            while (a < npoints - .5) #            local b = 0; #            while (b < npoints - .5)
#            local c = vlength(points[a] - points[b]); #            if (c > 0.00001 & c < elength) #            local elength = c; #            end
#            local b = b + 1; #            end #            local a = a + 1; #            end

#            debug concat("elength=", str(elength, 9, 9), "\n") showvtxs()

//Find planes
//#macro planes()
#            local a                                                                      = 0; #            while (a < npoints - .5)
#            local b                                                                      = a + 1; #            while (b < npoints - .5)
#            if (vlength(points[a] - points[b]) < elength + 0.00001) #            local c = b + 1; #            while (c < npoints - .5)
#                if (vlength(points[a] - points[c]) < elength + 0.00001)
#                    local n                                 = vnormalize(vcross(points[b] - points[a], points[c] - points[a]));
#                    local d                                 = vdot(n, points[a]);
#                    if (d < 0) #                    local n = -n; #                    local d = -d; #                    end
#                        local f                                                                = 1;
#                        local e                                                                = 0; #                        while (e < npoints - .5)
#                        if (vdot(n, points[e]) > d + 0.00001) #                        local f = 0; #                        end
#                            local e = e + 1; #                            end
#                            if (f)
#                                declare ld = d;
                                                                addface(n, d) //plane { n, d }
#                                end
#                                end
#                                local c = c + 1; #                                end #                                end
#                                local b = b + 1; #                                end
#                                local a = a + 1; #                                end
#                                end

            protected : static double tau;
        static double                 sq2;
        static double                 sq297;
        static double                 xi;
        /*
#declare tau=(1+sqrt(5))/2;
#declare sq2=sqrt(2);
#declare sq297=sqrt(297);
#declare xi=(pow(sq297+17,1/3)-pow(sq297-17,1/3)-1)/3;
#declare sqweird=sqrt(tau-5/27);
#declare ouch=pow((tau+sqweird)/2,1/3)+pow((tau-sqweird)/2,1/3);
#declare alfa=ouch-1/ouch;
#declare veta=(ouch+tau+1/ouch)*tau;
 */
        void set_name(const std::string &name);
        void add_points_even_sign(double x, double y, double z);
        void add_points_sign(double x0, double y0, double z0, double x1, double y1, double z1);
        void auto_faces();

    private:
        std::string m_name;
    };

    class tetrahedron : public polyhedron
    {
        tetrahedron()
            : polyhedron("tetrahedron")
        {
            add_points_even_sign(1.0, 1.0, 1.0);
            auto_faces();
        }
    };

    class hexahedron : public polyhedron
    {
        hexahedron()
            : polyhedron("hexahedron")
        {
            add_points_sign(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
            auto_faces();
        }
    };

    class octahedron()
        : public polyhedron{
              octahedron() : polyhedron("octahedron"){
                  add_even_permutations_sign(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    auto_faces();
}
};

class dodecahedron()
    : public polyhedron{
          dodecahedron() : polyhedron("dodecahedron"){
              add_points_sign(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
add_even_permutations_sign(0.0, 1.0 / tau, tau, 0.0, 1.0, 1.0);
auto_faces();
}
};

class icosahedron()
    : public polyhedron{
          icosahedron() : polyhedron("icosahedron"){
              add_even_permutations_sign(0.0, 1.0, tau, 0.0, 1.0, 1.0);
auto_faces();
}
}
;

class weirdahedron()
    : public polyhedron{
          weirdahedron() : polyhedron("weirdahedron"){
              add_permutations_sign(1.0, 2.0, 3.0, 1.0, 1.0, 1.0);
auto_faces();
}
}
;

class cuboctahedron()
    : public polyhedron{
          cuboctahedron() : polyhedron("cuboctahedron"){
              add_even_permutations_sign(0.0, 1.0, 1.0, 0.0, 1.0, 1.0);
auto_faces();
}
}
;

class icosidodecahedron()
    : public polyhedron{
          cuboctahedron(){
              add_even_permutations_sign(0.0, 0.0, 2.0 * tau, 0.0, 0.0, 1.0);
add_even_permutations_sign(1.0, tau, 1.0 + tau, 1.0, 1.0, 1.0);
auto_faces();
}
}
;

class truncated_tetrahedron()
    : public polyhedron{
          truncated_tetrahedron(bool augmentation) : polyhedron(augmentation ? "augmented truncated tetrahedron" : "truncated tetrahedron"){
              add_even_permutations_even_sign(1.0, 1.0, 3.0);
if (augmentation)
{
    augment(6, points()[0], points()[1], points()[4]);
}
auto_faces();
}
}
;

class truncated_hexahedron()
    : public polyhedron{
          truncated_hexahedron(bool augmentation){
              add_even_permutations_sign(sq2 - 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);

switch (augmentation)
{
    case 2:
        //set_name("biaugmented truncated hexahedron");
        augment(8, points()[7], points()[23], points()[22]);

    case 1:
        //set_name("augmented truncated hexahedron");
        augemnt(8, points()[16], points()[0], points()[1]);
        break;

    case 0:
        //set_name("truncated hexahedron");
        break;
}

auto_faces();
}
}
;

class truncated_octahedron()
    : public polyhedron{
          truncated_octahedron() : polyhedron("truncated octahedron"){
              add_permutations_sign(0.0, 1.0, 2.0, 0.0, 1.0, 1.0);
auto_faces();
}
}
;

class truncated_dodecahedron()
    : public polyhedron{
          truncated_dodecahedron(bool augmentation){
              add_even_permutations_sign(0.0, 1.0 / tau, 2.0 + tau, 0.0, 1.0, 1.0);
add_even_permutations_sign(1.0 / tau, tau, 2.0 * tau, 1.0, 1.0, 1.0);
add_even_permutations_sign(tau, 2.0, 1.0 + tau, 1.0, 1.0, 1.0);

if (augmentation)
{
    augment(10, points()[50], points()[58], points()[34]); // towards (tau,-1,0) -- common to all

    switch (augmentation)
    {
        case 2:
            set_name("biaugmented truncated hexahedron");
            augment(8, points()[7], points()[23], points()[22]);
            break;

        case 1:
            set_name("augmented truncated hexahedron");
            augemnt(8, points()[16], points()[0], points()[1]);
            break;

        case 0:
            set_name("truncated hexahedron");
    }
}
auto_faces();
}
}
;

#                                macro truncateddodecahedron(augmentation)
addevenpermssgn(<0, 1 / tau, 2 + tau>, <0, 1, 1>)
    addevenpermssgn(<1 / tau, tau, 2 * tau>, <1, 1, 1>)
        addevenpermssgn(<tau, 2, 1 + tau>, <1, 1, 1>)
#                                if (augmentation)
            augment(10, points[50], points[58], points[34]) // towards (tau,-1,0) -- common to all
#                                    switch (augmentation)
#                                    case (3)augment(10, points[54], points[38], points[14]) // towards (-1,0,tau) -- on tri
#                                    case (2)augment(10, points[40], points[48], points[24]) // towards (0,phi,-1) -- on metadi and tri
#                                    break
#                                    case (-2)augment(10, points[32], points[10], points[9]) // towards (-tau,1,0) -- on paradi
#                                    end
#                                    end
    autoface()
#                                    end

#                                    macro truncatedicosahedron()
        addevenpermssgn(<0, 1, 3 * tau>, <0, 1, 1>)
            addevenpermssgn(<2, 1 + 2 * tau, tau>, <1, 1, 1>)
                addevenpermssgn(<1, 2 + tau, 2 * tau>, <1, 1, 1>)
                    autoface()
#                                    end

#                                    macro rhombicuboctahedron()
                        addevenpermssgn(<1 + sq2, 1, 1>, <1, 1, 1>)
                            autoface()
#                                    end

#                                    macro truncatedcuboctahedron()
                                addpermssgn(<1, 1 + sq2, 1 + sq2 * 2>, <1, 1, 1>)
                                    autoface()
#                                    end

#                                    macro rhombicosidodecahedron()
                                        addevenpermssgn(<1, 1, 1 + 2 * tau>, <1, 1, 1>)
                                            addevenpermssgn(<tau, 2 * tau, 1 + tau>, <1, 1, 1>)
                                                addevenpermssgn(<2 + tau, 0, 1 + tau>, <1, 0, 1>)
                                                    autoface()
#                                    end

#                                    macro truncatedicosidodecahedron()
                                                        addevenpermssgn(<1 / tau, 1 / tau, 3 + tau>, <1, 1, 1>)
                                                            addevenpermssgn(<2 / tau, tau, 1 + 2 * tau>, <1, 1, 1>)
                                                                addevenpermssgn(<1 / tau, 1 + tau, 3 * tau - 1>, <1, 1, 1>)
                                                                    addevenpermssgn(<2 * tau - 1, 2, 2 + tau>, <1, 1, 1>)
                                                                        addevenpermssgn(<tau, 3, 2 * tau>, <1, 1, 1>)
                                                                            autoface()
#                                    end

#                                    macro snubhexahedron(s)
                                                                                addpermsaltsgn(<1, 1 / xi, xi> *s)
                                                                                    autoface()
#                                    end

#                                    macro snubdodecahedron(s)
                                                                                        addevenpermsevensgn(<2 * alfa, 2, 2 * veta> *s)
                                                                                            addevenpermsevensgn(<alfa + veta / tau + tau, -alfa * tau + veta + 1 / tau, alfa / tau + veta * tau - 1> *s)
                                                                                                addevenpermsevensgn(<-alfa / tau + veta * tau + 1, -alfa + veta / tau - tau, alfa * tau + veta - 1 / tau> *s)
                                                                                                    addevenpermsevensgn(<-alfa / tau + veta * tau - 1, alfa - veta / tau - tau, alfa * tau + veta + 1 / tau> *s)
                                                                                                        addevenpermsevensgn(<alfa + veta / tau - tau, alfa * tau - veta + 1 / tau, alfa / tau + veta * tau + 1> *s)
                                                                                                            autoface()
#                                    end

#                                    macro rhombicdodecahedron()
                                                                                                                cuboctahedron() dual()
#                                    end

#                                    macro rhombictriacontahedron()
                                                                                                                    icosidodecahedron() dual()
#                                    end

#                                    macro triakistetrahedron()
                                                                                                                        truncatedtetrahedron(0) dual()
#                                    end

#                                    macro triakisoctahedron()
                                                                                                                            truncatedhexahedron(0) dual()
#                                    end

#                                    macro tetrakishexahedron()
                                                                                                                                truncatedoctahedron() dual()
#                                    end

#                                    macro triakisicosahedron()
                                                                                                                                    truncateddodecahedron(0) dual()
#                                    end

#                                    macro pentakisdodecahedron()
                                                                                                                                        truncatedicosahedron() dual()
#                                    end

#                                    macro deltoidalicositetrahedron()
                                                                                                                                            rhombicuboctahedron() dual()
#                                    end

#                                    macro disdyakisdodecahedron()
                                                                                                                                                truncatedcuboctahedron() dual()
#                                    end

#                                    macro deltoidalhexecontahedron()
                                                                                                                                                    rhombicosidodecahedron() dual()
#                                    end

#                                    macro disdyakistriacontahedron()
                                                                                                                                                        truncatedicosidodecahedron() dual()
#                                    end

#                                    macro pentagonalicositetrahedron(s)
                                                                                                                                                            snubhexahedron(s) dual()
#                                    end

#                                    macro pentagonalhexecontahedron(s)
                                                                                                                                                                snubdodecahedron(s) dual()
#                                    end

//>>>>>>>>>>>>>>>>> changed AGK  [20041101]
#                                    macro polygon_vtx(n)
#                                    local i = 0;
#                                    while (i < n - .5)
                                                                                                                                                                    addpoint(<cos(i * 2 * pi / n), sin(i * 2 * pi / n), 0>)
#                                    local i = i + 1;
#                                    end
#                                    end
#                                    macro rprism_vtx(n)
#                                    local a = sqrt((1 - cos(2 * pi / n)) / 2);
#                                    local b = 0; #                                    while (b < n - .5)
                                                                                                                                                                        addpointssgn(<sin(2 * pi * b / n), cos(2 * pi * b / n), a>, <0, 0, 1>)
#                                    local b = b + 1; #                                    end
#                                    end
#                                    macro antiprism_vtx(n)
#                                    local a = sqrt((cos(pi / n) - cos(2 * pi / n)) / 2);
#                                    local b = 0; #                                    while (b < 2 * n - .5)
                                                                                                                                                                            addpoint(<sin(pi *b / n), cos(pi *b / n), a>)
#                                    local a = -a; #                                    local b = b + 1; #                                    end
#                                    end
#                                    macro rprism(n)
                                                                                                                                                                                rprism_vtx(n) autoface()
#                                    end
#                                    macro antiprism(n)
                                                                                                                                                                                    antiprism_vtx(n)
                                                                                                                                                                                        autoface()
#                                    end
//<<<<<<<<<<<<<<<<< changed AGK  [20041101]

#                                    macro bipyramid(n)
                                                                                                                                                                                            rprism(n) dual()
#                                    end

#                                    macro trapezohedron(n)
                                                                                                                                                                                                antiprism(n) dual()
#                                    end

//>>>>>>>>>>>>>>>>> added AGK  [20041101]
#                                    macro augment(n, va, vb, vc) // on an n-face with 3 adjacent vtxs, add a pyramid or a cupola
#                                    local veci = va - vb; #                                    local vecj = vc - vb; #                                    local veck = vlength(vc - vb) * vnormalize(vcross(vc - vb, va - vb));
#                                    switch (n)
#                                    case (3)addpoint((va + vb + vc) / 3 + sqrt(2 / 3) * veck) #                                    break
#                                    case (4)addpoint((va + vc) / 2 + sqrt(1 / 2) * veck) #                                    break
#                                    case (5)addpoint(vb + (2 + tau) / 5 * (veci + vecj) + sqrt((3 - tau) / 5) * veck) #                                    break
#                                    case (6)
                                                                                                                                                                                                    addpoint(vb + 1 / 3 * veci + 2 / 3 * vecj + sqrt(2 / 3) * veck)
                                                                                                                                                                                                        addpoint(vb + 4 / 3 * veci + 2 / 3 * vecj + sqrt(2 / 3) * veck)
                                                                                                                                                                                                            addpoint(vb + 4 / 3 * veci + 5 / 3 * vecj + sqrt(2 / 3) * veck)
#                                    break
#                                    case (8)
                                                                                                                                                                                                                addpoint(vb + sqrt(1 / 2) * veci + vecj + sqrt(1 / 2) * veck)
                                                                                                                                                                                                                    addpoint(vb + (1 + sqrt(1 / 2)) * veci + vecj + sqrt(1 / 2) * veck)
                                                                                                                                                                                                                        addpoint(vb + (1 + sqrt(1 / 2)) * veci + (1 + sq2) * vecj + sqrt(1 / 2) * veck)
                                                                                                                                                                                                                            addpoint(vb + (2 + sqrt(1 / 2)) * veci + (1 + sq2) * vecj + sqrt(1 / 2) * veck)
#                                    break
#                                    case (10)
                                                                                                                                                                                                                                addpoint(vb + (0.2 + 0.6 * tau) * veci + (0.8 + 0.4 * tau) * vecj + sqrt((3 - tau) / 5) * veck)
                                                                                                                                                                                                                                    addpoint(vb + (1.2 + 0.6 * tau) * veci + (0.8 + 0.4 * tau) * vecj + sqrt((3 - tau) / 5) * veck)
                                                                                                                                                                                                                                        addpoint(vb + (1.2 + 1.6 * tau) * veci + (0.8 + 1.4 * tau) * vecj + sqrt((3 - tau) / 5) * veck)
                                                                                                                                                                                                                                            addpoint(vb + (1.2 + 1.6 * tau) * veci + (1.8 + 1.4 * tau) * vecj + sqrt((3 - tau) / 5) * veck)
                                                                                                                                                                                                                                                addpoint(vb + (1.2 + 0.6 * tau) * veci + (0.8 + 1.4 * tau) * vecj + sqrt((3 - tau) / 5) * veck)
#                                    break
#                                    end
#                                    end
#                                    macro rotateabout(raxis, rangle, va) // raxis must be a unit vector
                                                                                                                                                                                                                                                    (vdot(raxis, va) * raxis + cos(rangle) * (va - vdot(raxis, va) * raxis) + sin(rangle) * (vcross(raxis, va)))
#                                    end
#                                    macro rotate_vtxs(raxis, rangle, thresh) // all points in the halfspace v.raxis <= tresh
#                                    local i = 0;
#                                    while (i < npoints - .5)
#                                    if (vdot(points[i], raxis) < thresh + 0.01)
#                                        declare points[i] = rotateabout(raxis, pi * rangle / 180, points[i]);
#                                        end // if
#                                        local i = i + 1;
#                                        end //while
#                                        end
#                                        macro drop_vtx(n)
#                                        declare npoints = npoints - 1;
#                                        if (n < npoints)
#                                            declare points[n] = points[npoints];
#                                            end
#                                            end
#                                            macro drop_halfspace(normalvector, thresh) // all points in the halfspace v.raxis < tresh
#                                            local i = 0;
#                                            while (i < npoints - .5)
#                                            if (vdot(points[i], normalvector) < thresh - 0.01)
#                                                debug concat("Drop vtx ", str(i, 0, 0), " of ", str(npoints, 0, 0), " <", str(points[i].x, 0, 3), ",", str(points[i].y, 0, 3), ",", str(points[i].z, 0, 3), "> (", str(vdot(points[i], normalvector), 0, 7), ")\n")
                                                                                                                                                                                                                                                        drop_vtx(i)
#                                            else
#                                                debug concat("Keep vtx ", str(i, 0, 0), " of ", str(npoints, 0, 0), " <", str(points[i].x, 0, 3), ",", str(points[i].y, 0, 3), ",", str(points[i].z, 0, 3), "> (", str(vdot(points[i], normalvector), 0, 7), ")\n")
#                                                local i = i + 1;
#                                                end
#                                                end
#                                                end
#                                                macro autobalance() // moves the centre of gravity (cog) of the vertices to the origin
#                                                local cog = < 0, 0, 0 > ;
#                                                local i   = 0;
#                                                while (i < npoints - .5)
#                                                local cog = cog + points[i];
#                                                local i   = i + 1;
#                                                end
#                                                local cog = cog / npoints;
#                                                local i   = 0;
#                                                while (i < npoints - .5)
#                                                declare points[i] = points[i] - cog;
#                                                local i           = i + 1;
#                                                end
#                                                end

zz

//--------------- macros to find "sporadic" Johnson solids via iterative optimisation kludge
#                                                declare el      = 1;
#                                                declare edgelen = array[120][120];
#                                                declare forces  = array[120];
#                                                macro addedge(a, b, len)
#                                                declare edgelen[a][b] = len;
#                                                declare edgelen[b][a] = len;
#                                                end
#                                                macro make_triangle(a, b, c)
    addedge(a,b,el)    addedge(a,c,el) addedge(b,c,el)
#                                                end
#                                                macro make_square(a, b, c, d)
    addedge(a,b,el) addedge(b,c,el) addedge(c,d,el) addedge(d,a,el) addedge(a,c,sq2*el) addedge(b,d,sq2*el)
#                                                end
#                                                macro make_lune(a, b, c, d, e, f) // a and d are points of lune
    make_triangle(a,b,f) make_square(b,c,e,f) make_triangle(c,d,e)
#                                                end
#                                                macro optimise(gen_threshold, force_threshold)
#                                                local gen = 0; #                                                local maxforce = force_threshold + 1;
#                                                while ((gen < gen_threshold) & (maxforce > force_threshold))
#                                                debug concat("Gen ", str(gen, 0, 0), " ")
//     showvtxs()
#                                                local maxforce = -999;
#                                                local i        = 0;
#                                                while (i < npoints)
#                                                declare forces[i] = < 0, 0, 0 > ;
#                                                local j           = 0;
#                                                while (j < npoints)
#                                                ifdef(edgelen[i][j])
#                                                    local dist        = vlength(points[i] - points[j]);
#                                                    declare forces[i] = forces[i] + (dist - edgelen[i][j]) * (points[j] - points[i]);
//                 #debug concat("Edge ",str(i,0,0)," & ",str(j,0,0)," has length ",str(dist,5,5)," want length ",str(edgelen[i][j],5,5),"\n")
#                                                    end
#                                                    local j = j + 1;
#                                                    end
#                                                    if (maxforce < vlength(forces[i])) #                                                    local maxforce = vlength(forces[i]); #                                                    end
#                                                        local i = i + 1;
#                                                        end
#                                                        debug concat("maxforce=", str(maxforce, 9, 9), "\n")
#                                                        local i = 0;
#                                                        while (i < npoints)
#                                                        declare points[i] = points[i] + .1 * forces[i];
#                                                        local i           = i + 1;
#                                                        end
#                                                        local gen = gen + 1;
#                                                        end
#                                                        end


// Johnson solids
// J1 = square_pyramid (octahedron with vtx dropped)
#                                                        macro square_pyramid()
  addevenpermssgn(<1,0,0>,<1,0,0>) drop_vtx(99)
  autobalance()  autoface()
#                                                        end
// J2 = pentagonal_pyramid (six vtxs of an icosahedron)
#                                                        macro pentagonal_pyramid()
    addevenpermssgn(<0,1,tau>,<0,1,1>) drop_halfspace(points[0],0)
    autobalance()  autoface()
#                                                        end
// ----------------- cuboctahedron modifications J - 3, 18, 22, 27, 35, 36, 44
// J3 = triangular_cupola (9 vtxs of a cuboctahedron)
#                                                        macro triangular_cupola()
    polygon_vtx(6)
    augment(6,points[0],points[1],points[2])
    autobalance()  autoface()
#                                                        end
#                                                        macro triangular_gyrobicupola() //actually a cuboctahedron
    polygon_vtx(6)
    augment(6,points[0],points[1],points[2])
    augment(6,points[2],points[1],points[0])
    autobalance()  autoface()
#                                                        end
#                                                        macro elongated_triangular_cupola() //J18
    rprism_vtx(6)
    augment(6,points[1],points[3],points[5])
    autobalance()  autoface()
#                                                        end
#                                                        macro gyroelongated_triangular_cupola() //J22
    antiprism_vtx(6)
    augment(6,points[1],points[3],points[5])
    autobalance()  autoface()
#                                                        end
#                                                        macro triangular_orthobicupola() //J27
    polygon_vtx(6)
    augment(6,points[0],points[1],points[2])
    augment(6,points[3],points[2],points[1])
    autobalance()  autoface()
#                                                        end
#                                                        macro elongated_triangular_orthobicupola() //J35
    rprism_vtx(6)
    augment(6,points[1],points[3],points[5])
    augment(6,points[6],points[4],points[2])
    autobalance()  autoface()
#                                                        end
#                                                        macro elongated_triangular_gyrobicupola() //J36
    rprism_vtx(6)
    augment(6,points[1],points[3],points[5])
    augment(6,points[4],points[2],points[0])
    autobalance()  autoface()
#                                                        end
#                                                        macro gyroelongated_triangular_bicupola() //J44
    antiprism_vtx(6)
    augment(6,points[1],points[3],points[5])
    augment(6,points[4],points[2],points[0])
    autobalance()  autoface()
#                                                        end

// two triangular prisms
#                                                        macro gyrobifastigium() // J26
    addpointssgn(<1,1,0>,<1,1,0>)
    addpointssgn(<1,0,sqrt(3)>,<1,0,0>)
    addpointssgn(<0,1,-sqrt(3)>,<0,1,0>)
    autobalance()  autoface()
#                                                        end   
//---------------- miscellaneous cut and pasting
#                                                        macro elongated_pyramid(n) // J7-9 (for n=3,4,5)
    rprism_vtx(n)
    augment(n,points[4],points[2],points[0])
    autobalance()  autoface()
#                                                        end

#                                                        macro dipyramid(n) // J12 (n=3) and J13 (n=5)
    polygon_vtx(n)
    augment(n,points[0],points[1],points[2])
    augment(n,points[2],points[1],points[0])
    autobalance()  autoface()
#                                                        end
#                                                        macro elongated_dipyramid(n) // J14-16 (for n=3,4,5)
    rprism_vtx(n)
    augment(n,points[4],points[2],points[0])
    augment(n,points[1],points[3],points[5])
    autobalance()  autoface()
#                                                        end
#                                                        macro elongated_triangular_dipyramid() elongated_dipyramid(3) #                                                        end // J7
#                                                        macro elongated_square_dipyramid() elongated_dipyramid(3) #                                                        end     // J8
#                                                        macro elongated_pentagonal_dipyramid() elongated_dipyramid(3) #                                                        end // J9

// ----------------- rhombicuboctahedron modifications J - 4, 19, 23, 28, 29, 37, 45
#                                                        macro rhombicuboctahedron_mod(j_number)
    addevenpermssgn(<1+sq2,1,1>,<1,1,1>)
#                                                        local raxis      = x;
#                                                        local edgelen    = 2;
#                                                        local oct_radius = sqrt(2 * sq2 + 4);
    // drop hemisphere for 6, 21, 25 (have single rotunda)
#                                                        if (j_number = 4) drop_halfspace(raxis, 1) #                                                        end
#                                                            if (j_number <= 23) drop_halfspace(raxis, -1) #                                                            end
    // stretch and twist
#                                                                local stretch = 0; #                                                                local twist = 0;
#                                                                switch (j_number)
#                                                                case (29)
#                                                                local twist = 45;
#                                                                case (28)
#                                                                local stretch = -edgelen;
#                                                                break
#                                                                case (37)
#                                                                local twist = 45;
#                                                                break
#                                                                case (23) #                                                                case (45)
#                                                                local twist   = 22.5;
#                                                                local stretch = oct_radius * 2 * sqrt((cos(pi / 8) - cos(2 * pi / 8)) / 2) - edgelen; // borrowed from antiprism_vtx
#                                                                end                                                                                   //switch
#                                                                if (stretch != 0)                                                                     // lower northern hemisphere
#                                                                    local i = 0;
#                                                                    while (i < npoints - .5)
#                                                                    if ((stretch = -2) & (vdot(points[i], raxis) = 1))
                drop_vtx(i)
#                                                                    else
#                                                                        if (vdot(points[i], raxis) > 0)
#                                                                            declare points[i] = points[i] + stretch * raxis;
#                                                                            end // if
#                                                                            local i = i + 1;
#                                                                            end             //if
#                                                                            end             //while
#                                                                            end             //if
#                                                                            if (twist != 0) // rotate southern hemisphere (incl equator)
        rotate_vtxs(raxis,twist,-1)
#                                                                                end
    autobalance()
#                                                                                end
// Now the named macros of these modified rhombicuboctahedron
#                                                                                macro square_cupola() rhombicuboctahedron_mod(4) autoface() #                                                                                end                  //  J4
#                                                                                macro elongated_square_cupola() rhombicuboctahedron_mod(19) autoface() #                                                                                end       //  J19
#                                                                                macro gyroelongated_square_cupola() rhombicuboctahedron_mod(23) autoface() #                                                                                end   //  J23
#                                                                                macro square_orthobicupola() rhombicuboctahedron_mod(28) autoface() #                                                                                end          //  J28
#                                                                                macro square_gyrobicupola() rhombicuboctahedron_mod(29) autoface() #                                                                                end           //  J29
#                                                                                macro elongated_square_gyrobicupola() rhombicuboctahedron_mod(37) autoface() #                                                                                end //  J37
#                                                                                macro gyroelongated_square_bicupola() rhombicuboctahedron_mod(45) autoface() #                                                                                end //  J45

#                                                                                macro elongated_square_cupola_alt() //  J19
    rprism_vtx(8)
    augment(8,points[4],points[2],points[0])
    autoface() #end

// J10.    (cap a square antiprism)
#                                                                                macro gyroelongated_square_pyramid()
  antiprism_vtx(4)
#                                                                                local va = points[1];
  addpoint(<0,0,-(abs(va.z)+1)>)
  autoface()
#                                                                                end
// J17.    (bicap a square antiprism)
#                                                                                macro gyroelongated_square_dipyramid()
  antiprism_vtx(4)
#                                                                                local va = points[1];
  addpoint(<0,0,abs(va.z)+1>)
  addpoint(<0,0,-(abs(va.z)+1)>)
  autoface()
#                                                                                end

// ----------------- icosahedron modifications
// J11.    (drop a vertex from an icosahedron)
#                                                                                macro gyroelongated_pentagonal_pyramid()
  addevenpermssgn(<0,1,tau>,<0,1,1>)
  drop_vtx(99)
  autoface()
#                                                                                end
// J62.    (drop 2 vertices from an icosahedron)
#                                                                                macro metabidiminished_icosahedron()
  addevenpermssgn(<0,1,tau>,<0,1,1>)
  drop_vtx(99)
  drop_vtx(6)
  autoface()
#                                                                                end
// J63.    (drop 3 vertices from an icosahedron)
#                                                                                macro tridiminished_icosahedron()
  addevenpermssgn(<0,1,tau>,<0,1,1>)
  drop_vtx(99)
  drop_vtx(6)
  drop_vtx(0)  // 5 OK too
  autoface()
#                                                                                end
// J64.    (drop 3 vertices from an icosahedron, add a tetrahedron)
#                                                                                macro augmented_tridiminished_icosahedron()
  addevenpermssgn(<0,1,tau>,<0,1,1>)
  drop_vtx(99)
  drop_vtx(6)
  drop_vtx(0)
  augment(3,points[1],points[7],points[8])
  autoface()
#                                                                                end

// -------------------- dodecahedron modifications: J58-61
#                                                                                macro augmented_dodecahedron() //J58
  addpointssgn(<1,1,1>,<1,1,1>)
  addevenpermssgn(<0,1/tau,tau>,<0,1,1>)
  augment(5,points[4],points[13],points[12])
  showvtxs()
  autobalance() autoface()
#                                                                                end
#                                                                                macro parabiaugmented_dodecahedron() //J59
  addpointssgn(<1,1,1>,<1,1,1>)
  addevenpermssgn(<0,1/tau,tau>,<0,1,1>)
  augment(5,points[4],points[13],points[12])
#                                                                                local a = points[npoints - 1];
  addpoint(-a)
  showvtxs()
  autobalance() autoface()
#                                                                                end
#                                                                                macro metabiaugmented_dodecahedron() //J60
  addpointssgn(<1,1,1>,<1,1,1>)
  addevenpermssgn(<0,1/tau,tau>,<0,1,1>)
  augment(5,points[4],points[13],points[12])
#                                                                                local a = points[npoints - 1];
  addpoint(<a.y,a.z,a.x>)
  showvtxs()
  autobalance() autoface()
#                                                                                end
#                                                                                macro triaugmented_dodecahedron() //J61
  addpointssgn(<1,1,1>,<1,1,1>)
  addevenpermssgn(<0,1/tau,tau>,<0,1,1>)
  augment(5,points[4],points[13],points[12])
#                                                                                local a = points[npoints - 1]; drop_vtx(999)
  addevenperms(a)
  showvtxs()
  autobalance() autoface()
#                                                                                end

// ----------------- icosidodecahedron modifications
// Modified icosidodecahedron, for J- 6, 21, 25, 34, 42, 43, 48; J32,33,40,41,47
#                                                                                macro icosidodecahedron_mod(j_number)
    addevenpermssgn(<0,0,2*tau>,<0,0,1>)
    addevenpermssgn(<1,tau,1+tau>,<1,1,1>)
#                                                                                local raxis     = vnormalize(<tau, 1, 0>);
#                                                                                local edgelen   = vlength(<0, 0, 2 * tau> - <1, tau, 1 + tau>);
#                                                                                local id_radius = 2 * tau;
    // drop hemisphere for 6, 21, 25 (have single rotunda)
#                                                                                if ((j_number <= 33) | (j_number = 40) | (j_number = 41) | (j_number = 47))
        drop_halfspace(raxis,0)
#                                                                                    if (j_number >= 32) // form a cupolarotunda
            augment(10,points[0],points[7],points[15])
#                                                                                        end
#                                                                                        end
    // stretch and twist
#                                                                                        local stretch = 0; #                                                                                        local twist = 0;
#                                                                                        switch (j_number)
#                                                                                        case (42) #                                                                                        case (40)
#                                                                                        local stretch = edgelen;
#                                                                                        case (34) #                                                                                        case (33)
#                                                                                        local twist = 36;
#                                                                                        break
#                                                                                        case (21) #                                                                                        case (43) #                                                                                        case (41)
#                                                                                        local stretch = edgelen;
#                                                                                        break
#                                                                                        case (25) #                                                                                        case (48) #                                                                                        case (47)
#                                                                                        local twist   = 18;
#                                                                                        local stretch = id_radius * 2 * sqrt((cos(pi / 10) - cos(2 * pi / 10)) / 2); // borrowed from antiprism_vtx
#                                                                                        end                                                                          //switch
#                                                                                        if (stretch > 0)                                                             // raise northern hemisphere, duplicate equator
#                                                                                            local i = 0; #                                                                                            local np = npoints;
#                                                                                            while (i < np - .5)
#                                                                                            switch (vdot(points[i], raxis))
#                                                                                            range(-0.01, 0.01)
//             #debug concat("Dupl. vtx ",str(i,0,0)," of ",str(npoints,0,0)," <",str(points[i].x,0,3),",",str(points[i].y,0,3),",",str(points[i].z,0,3),">\n")
                addpoint(points[i] + stretch*raxis)
#                                                                                            break
#                                                                                            range(0.01, 999)
//             #debug concat("Raise vtx ",str(i,0,0)," of ",str(npoints,0,0)," <",str(points[i].x,0,3),",",str(points[i].y,0,3),",",str(points[i].z,0,3),">\n")
#                                                                                            declare points[i] = points[i] + stretch * raxis;
#                                                                                            break
#                                                                                            end // switch
#                                                                                            local i = i + 1;
#                                                                                            end             //while
#                                                                                            end             //if
#                                                                                            if (twist != 0) // rotate southern hemisphere (incl equator)
        rotate_vtxs(raxis,twist,0)
#                                                                                                end
    showvtxs()
    autobalance() autoface()
#                                                                                                end

#                                                                                                macro pentagonal_rotunda() icosidodecahedron_mod(6) #                                                                                                end                   // J6. Half an icosidodecahedron
#                                                                                                macro elongated_pentagonal_rotunda() icosidodecahedron_mod(21) #                                                                                                end        // J21. Half an icosidodecahedron on a prism
#                                                                                                macro gyroelongated_pentagonal_rotunda() icosidodecahedron_mod(25) #                                                                                                end    // J25. Half an icosidodecahedron on an antiprism
#                                                                                                macro pentagonal_orthobirotunda() icosidodecahedron_mod(34) #                                                                                                end           // J34. Twisted icosidodecahedron
#                                                                                                macro elongated_pentagonal_gyrobirotunda() icosidodecahedron_mod(43) #                                                                                                end  // J43. Elongated icosidodecahedron
#                                                                                                macro elongated_pentagonal_orthobirotunda() icosidodecahedron_mod(42) #                                                                                                end // J42. Elongated twisted icosidodecahedron
#                                                                                                macro gyroelongated_pentagonal_birotunda() icosidodecahedron_mod(48) #                                                                                                end  // J48. Elongated semitwisted icosidodecahedron

#                                                                                                macro pentagonal_orthocupolarotunda() icosidodecahedron_mod(32) #                                                                                                end //J32
#                                                                                                macro pentagonal_gyrocupolarotunda() icosidodecahedron_mod(33) #                                                                                                end  //J32

//---------------------- pentagonal cupolae, bicupolae
#                                                                                                macro elongated_pentagonal_cupola()                                                                                                                                  //J20
    rprism_vtx(10)
    augment(10,points[4],points[2],points[0])
    autobalance()  autoface()
#                                                                                                end
#                                                                                                macro gyroelongated_pentagonal_cupola() //J24
    antiprism_vtx(10)
    augment(10,points[4],points[2],points[0])
    autobalance()  autoface()
#                                                                                                end

#                                                                                                macro pentagonal_orthobicupola() //J30
    polygon_vtx(10)
    augment(10,points[0],points[1],points[2])
    augment(10,points[3],points[2],points[1])
    autobalance()  autoface()
#                                                                                                end

#                                                                                                macro pentagonal_gyrobicupola() //J31
    polygon_vtx(10)
    augment(10,points[0],points[1],points[2])
    augment(10,points[2],points[1],points[0])
    autobalance()  autoface()
#                                                                                                end

#                                                                                                macro elongated_pentagonal_orthobicupola() //J38
    rprism_vtx(10)
    augment(10,points[4],points[2],points[0])
    augment(10,points[3],points[5],points[7])
    autobalance()  autoface()
#                                                                                                end

#                                                                                                macro elongated_pentagonal_gyrobicupola() //J39
    rprism_vtx(10)
    augment(10,points[4],points[2],points[0])
    augment(10,points[1],points[3],points[5])
    showvtxs()
    autobalance()  autoface()
#                                                                                                end
#                                                                                                macro gyroelongated_pentagonal_bicupola() //J46
    antiprism_vtx(10)
    augment(10,points[4],points[2],points[0])
    augment(10,points[1],points[3],points[5])
    autobalance()  autoface()
#                                                                                                end

// -------------------- side-capped prisms : J49-57
#                                                                                                macro augmented_prisms(n, facelist) // n=prism base, facelist=string with faces to cap
    rprism_vtx(n)
#                                                                                                local i = 1;
#                                                                                                while (i <= strlen(facelist))
#                                                                                                local facenum = mod(val(substr(facelist, i, 1)), n); // convert ith char given to a number 0..(n-1)
        augment(4,points[2*facenum+1],points[2*facenum],points[mod(2*facenum+2,2*n)])
//     #debug concat("Augment face ",str(facenum,0,0)," of ",str(n,0,0), " <",str(points[npoints-1].x,0,3),",",str(points[npoints-1].y,0,3),",",str(points[npoints-1].z,0,3),"> \n")
#                                                                                                local i       = i + 1;
#                                                                                                end
    autobalance()  autoface()
#                                                                                                end
#                                                                                                macro augmented_triangular_prism() augmented_prisms(3, "0") #                                                                                                end       // J49
#                                                                                                macro biaugmented_triangular_prism() augmented_prisms(3, "01") #                                                                                                end    // J50
#                                                                                                macro triaugmented_triangular_prism() augmented_prisms(3, "012") #                                                                                                end  // J51
#                                                                                                macro augmented_pentagonal_prism() augmented_prisms(5, "0") #                                                                                                end       // J52
#                                                                                                macro biaugmented_pentagonal_prism() augmented_prisms(5, "02") #                                                                                                end    // J53
#                                                                                                macro augmented_hexagonal_prism() augmented_prisms(6, "0") #                                                                                                end        // J54
#                                                                                                macro parabiaugmented_hexagonal_prism() augmented_prisms(6, "03") #                                                                                                end // J55
#                                                                                                macro metabiaugmented_hexagonal_prism() augmented_prisms(6, "02") #                                                                                                end // J56
#                                                                                                macro triaugmented_hexagonal_prism() augmented_prisms(6, "024") #                                                                                                end   // J57

// ----------------- rhombicosidodecahedron modifications
#                                                                                                macro pentagonal_cupola()                                                                                                                                              //J5
  addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
  addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
  addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
#                                                                                                local raxis = vnormalize(<tau, -1, 0>);
  drop_halfspace(raxis,3.077)
  autobalance()  autoface()
#                                                                                                end
#                                                                                                macro mogrified_rhombicosidodecahedron(mods) //J72-J83
    // mods is a 4-character string of D (drop), G (gyrate) and other (leave alone)
    addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
    addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
    addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
#                                                                                                local raxis    = array[5];
#                                                                                                local raxis[1] = vnormalize(<tau, -1, 0>);
#                                                                                                local raxis[2] = vnormalize(<-1, 0, tau>);
#                                                                                                local raxis[3] = vnormalize(<-1, 0, -tau>);
#                                                                                                local raxis[4] = -raxis[1];
#                                                                                                local i        = 1;
#                                                                                                while (i <= min(4, strlen(mods)))
#                                                                                                local modchar = substr(mods, i, 1);
#                                                                                                if (strcmp(modchar, "D") = 0) drop_halfspace(-raxis[i], -3.077) #                                                                                                end
#                                                                                                    if (strcmp(modchar, "G") = 0) rotate_vtxs(-raxis[i], 36, -3.077) #                                                                                                    end
#                                                                                                        local i = i + 1;
#                                                                                                        end
    autobalance()  autoface()
#                                                                                                        end

// #macro diminished_rhombicosidodecahedron()  //J76
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   drop_halfspace(-raxis,-3.077)
//   autobalance()  autoface()
// #end
// #macro tridiminished_rhombicosidodecahedron()   //J83
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   drop_halfspace(-raxis,-3.077)
//   #local raxis=vnormalize(<-1,0,-tau>);
//   drop_halfspace(-raxis,-3.077)
//   #local raxis=vnormalize(<-1,0,tau>);
//   drop_halfspace(-raxis,-3.077)
//   autobalance()  autoface()
// #end
// #macro metabidiminished_rhombicosidodecahedron()    //J81
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   drop_halfspace(-raxis,-3.077)
//   #local raxis=vnormalize(<-1,0,tau>);
//   drop_halfspace(-raxis,-3.077)
//   autobalance()  autoface()
// #end
// #macro parabidiminished_rhombicosidodecahedron()    //J80
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   drop_halfspace(-raxis,-3.077)
//   drop_halfspace( raxis,-3.077)
//   autobalance()  autoface()
// #end
// 
// #macro gyrate_rhombicosidodecahedron()  //J72
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   rotate_vtxs(-raxis,36,-3.077)
//   autobalance()  autoface()
// #end
// #macro trigyrate_rhombicosidodecahedron()   //J75
//   addevenpermssgn(<1,1,1+2*tau>,<1,1,1>)
//   addevenpermssgn(<tau,2*tau,1+tau>,<1,1,1>)
//   addevenpermssgn(<2+tau,0,1+tau>,<1,0,1>)
//   #local raxis=vnormalize(<tau,-1,0>);
//   rotate_vtxs(-raxis,36,-3.077)
//   #local raxis=vnormalize(<-1,0,-tau>);
//   rotate_vtxs(-raxis,36,-3.077)
//   #local raxis=vnormalize(<-1,0,tau>);
//   rotate_vtxs(-raxis,36,-3.077)
//   autobalance()  autoface()
// #end

////////////// sporadics

#                                                                                                        macro snub_disphenoid() // J84
    addpoint(<1,0,0>)  #local EQTR1=npoints-1;
    addpoint(<0,1,0>)  #local EQTR2=npoints-1;
    addpoint(<-1,0,0>) #local EQTR3=npoints-1;
    addpoint(<0,-1,0>) #local EQTR4=npoints-1;
    addpoint(<1,0,1>)  #local NORTH1=npoints-1;
    addpoint(<-1,0,1>) #local NORTH2=npoints-1;
    addpoint(<0,1,-1>) #local SOUTH1=npoints-1;
    addpoint(<0,-1,-1>)    #local SOUTH2=npoints-1;
    make_triangle(EQTR1,EQTR2,NORTH1)  make_triangle(EQTR1,EQTR2,SOUTH1)
    make_triangle(EQTR1,EQTR4,NORTH1)  make_triangle(EQTR1,EQTR4,SOUTH2)
    make_triangle(EQTR2,EQTR3,NORTH2)  make_triangle(EQTR2,EQTR3,SOUTH1)
    make_triangle(EQTR3,EQTR4,NORTH2)  make_triangle(EQTR3,EQTR4,SOUTH2)
    addedge(NORTH1,NORTH2,1)
    addedge(SOUTH1,SOUTH2,1)
    optimise(100,0.000001)
    autobalance()  autoface()
#                                                                                                        end

#                                                                                                        macro snub_square_antiprism() // J85
    addpoint(<sq2,0,0>)    #local E1=npoints-1;
    addpoint(<1,1,0>)  #local E2=npoints-1;
    addpoint(<0,sq2,0>)    #local E3=npoints-1;
    addpoint(<-1,1,0>) #local E4=npoints-1;
    addpoint(<-sq2,0,0>)   #local E5=npoints-1;
    addpoint(<-1,-1,0>)    #local E6=npoints-1;
    addpoint(<0,-sq2,0>)   #local E7=npoints-1;
    addpoint(<1,-1,0>) #local E8=npoints-1;
    addpoint(<.5,.5,1>)    #local N1=npoints-1;
    addpoint(<-.5,.5,1>)   #local N2=npoints-1;
    addpoint(<-.5,-.5,1>)  #local N3=npoints-1;
    addpoint(<.5,-.5,1>)   #local N4=npoints-1;
    addpoint(<1/sq2,0,-1>) #local S1=npoints-1;
    addpoint(<0,1/sq2,-1>) #local S2=npoints-1;
    addpoint(<-1/sq2,0,-1>)    #local S3=npoints-1;
    addpoint(<0,-1/sq2,-1>)    #local S4=npoints-1;
    make_triangle(E1,E2,N1)    make_triangle(E1,E2,S1)
    make_triangle(E2,E3,N1)    make_triangle(E2,E3,S2)
    make_triangle(E3,E4,N2)    make_triangle(E3,E4,S2)
    make_triangle(E4,E5,N2)    make_triangle(E4,E5,S3)
    make_triangle(E5,E6,N3)    make_triangle(E5,E6,S3)
    make_triangle(E6,E7,N3)    make_triangle(E6,E7,S4)
    make_triangle(E7,E8,N4)    make_triangle(E7,E8,S4)
    make_triangle(E8,E1,N4)    make_triangle(E8,E1,S1)
    addedge(N1,N2,1)       addedge(N2,N3,1)    addedge(N1,N3,sq2)
    addedge(N3,N4,1)       addedge(N4,N1,1)    addedge(N2,N4,sq2)
    addedge(S1,S2,1)       addedge(S2,S3,1)    addedge(S1,S3,sq2)
    addedge(S3,S4,1)       addedge(S4,S1,1)    addedge(S2,S4,sq2)
    optimise(400,0.00000001)
    autobalance()
    autoface()
#                                                                                                        end

#                                                                                                        macro sphenocoronae(n) // J86 & J87
    addpoint(<1,0,0>)  #local E1=npoints-1;
    addpoint(<.5,1,0>) #local E2=npoints-1;
    addpoint(<-.5,1,0>)    #local E3=npoints-1;
    addpoint(<-1,0,0>) #local E4=npoints-1;
    addpoint(<-.5,-1,0>)   #local E5=npoints-1;
    addpoint(<.5,-1,0>)    #local E6=npoints-1;
    addpoint(<.5,0,1>) #local N1=npoints-1;
    addpoint(<-.5,0,1>)    #local N2=npoints-1;
    addpoint(<0,.5,-1>)    #local S1=npoints-1;
    addpoint(<0,-.5,-1>)   #local S2=npoints-1;
    make_lune(E1,E2,E3,E4,N2,N1)
    make_lune(E4,E5,E6,E1,N1,N2)
    make_triangle(E1,E2,S1) make_triangle(E2,E3,S1) make_triangle(E3,E4,S1)
    make_triangle(E4,E5,S2) make_triangle(E5,E6,S2) make_triangle(E6,E1,S2)
    addedge(S1,S2,1)
    optimise(400,0.00000001)
#                                                                                                        if (n = 87) augment(4, points[E2], points[E3], points[N2]) #                                                                                                        end
    autobalance()
    autoface()
#                                                                                                            end
#                                                                                                            macro sphenocorona() // J86
    sphenocoronae(86)
#                                                                                                            end
#                                                                                                            macro augmented_sphenocorona() // J87
    sphenocoronae(87)
#                                                                                                            end
#                                                                                                            macro augmented_sphenocorona_old() // J87
    addpoint(<1,0,0>)  #local E1=npoints-1;
    addpoint(<.5,1,0>) #local E2=npoints-1;
    addpoint(<-.5,1,0>)    #local E3=npoints-1;
    addpoint(<-1,0,0>) #local E4=npoints-1;
    addpoint(<-.5,-1,0>)   #local E5=npoints-1;
    addpoint(<.5,-1,0>)    #local E6=npoints-1;
    addpoint(<.5,0,1>) #local N1=npoints-1;
    addpoint(<-.5,0,1>)    #local N2=npoints-1;
    addpoint(<0,.5,-1>)    #local S1=npoints-1;
    addpoint(<0,-.5,-1>)   #local S2=npoints-1;
    make_lune(E1,E2,E3,E4,N2,N1)
    make_lune(E4,E5,E6,E1,N1,N2)
    make_triangle(E1,E2,S1) make_triangle(E2,E3,S1) make_triangle(E3,E4,S1)
    make_triangle(E4,E5,S2) make_triangle(E5,E6,S2) make_triangle(E6,E1,S2)
    addedge(S1,S2,1)
    addpoint(<0,1,1>)  #local A=npoints-1;
    make_triangle(A,E2,E3) make_triangle(A,N2,N1)
    optimise(400,0.00000001)
    autobalance()
    autoface()
#                                                                                                            end
#                                                                                                            macro sphenomegacorona() // J88
    addpoint(<1.3,0,0.1>)  #local E1=npoints-1;
    addpoint(<.5,.6,0>)    #local E2=npoints-1;
    addpoint(<-.5,.6,0>)   #local E3=npoints-1;
    addpoint(<-1.3,0,0.1>) #local E4=npoints-1;
    addpoint(<-.5,-.6,0>)  #local E5=npoints-1;
    addpoint(<.5,-.6,0>)   #local E6=npoints-1;
    addpoint(<.5,0,.7>)    #local N1=npoints-1;
    addpoint(<-.5,0,.7>)   #local N2=npoints-1;
    addpoint(<0,.5,-.9>)   #local S1=npoints-1;
    addpoint(<-.8,0,-.8>)  #local S2=npoints-1;
    addpoint(<0,-.5,-.9>)  #local S3=npoints-1;
    addpoint(<.8,0,-.8>)   #local S4=npoints-1;
    make_lune(E1,E2,E3,E4,N2,N1)
    make_lune(E4,E5,E6,E1,N1,N2)
    make_triangle(E1,E2,S4) make_triangle(E2,E3,S1) make_triangle(E3,E4,S2)
    make_triangle(E4,E5,S2) make_triangle(E5,E6,S3) make_triangle(E6,E1,S4)
    make_triangle(S1,S2,S3)    make_triangle(S3,S4,S1)
    optimise(400,0.000001)
// showvtxs()
    autobalance()
    autoface()
#                                                                                                            end

#                                                                                                            macro hebesphenomegacorona() // J89
    addpoint(< 1.10, 0.00, 0.20>)  #local E1=npoints-1;
    addpoint(< 0.50, 0.72,-0.15>)  #local E2=npoints-1;
    addpoint(<-0.50, 0.72,-0.15>)  #local E3=npoints-1;
    addpoint(<-1.10, 0.00, 0.20>)  #local E4=npoints-1;
    addpoint(<-0.50,-0.72,-0.15>)  #local E5=npoints-1;
    addpoint(< 0.50,-0.72,-0.15>)  #local E6=npoints-1;
    addpoint(< 0.50, 0.50, 0.83>)  #local N1=npoints-1;
    addpoint(<-0.50, 0.50, 0.83>)  #local N2=npoints-1;
    addpoint(<-0.50,-0.50, 0.83>)  #local N3=npoints-1;
    addpoint(< 0.50,-0.50, 0.83>)  #local N4=npoints-1;
    addpoint(< 0.00, 0.50,-0.99>)  #local S1=npoints-1;
    addpoint(<-0.84, 0.00,-0.76>)  #local S2=npoints-1;
    addpoint(< 0.00,-0.50,-0.99>)  #local S3=npoints-1;
    addpoint(< 0.84, 0.00,-0.76>)  #local S4=npoints-1;
    make_lune(E1,E2,E3,E4,N2,N1)
    make_lune(E4,E5,E6,E1,N4,N3)
    make_lune(E1,N1,N2,E4,N3,N4)

    make_triangle(E1,E2,S4) make_triangle(E2,E3,S1) make_triangle(E3,E4,S2)
    make_triangle(E4,E5,S2) make_triangle(E5,E6,S3) make_triangle(E6,E1,S4)
    make_triangle(S1,S2,S3)
    make_triangle(S3,S4,S1)
    optimise(400,0.000001)
    showvtxs()
    autobalance()
    autoface()
#                                                                                                            end

#                                                                                                            macro disphenocingulum() //  J90
    addpoint(< 0.00, 0.50, 1.10>)  #local NN1=npoints-1;
    addpoint(< 0.00,-0.50, 1.10>)  #local NN2=npoints-1;
    addpoint(< 0.00, 1.12, 0.33>)  #local N1=npoints-1;
    addpoint(< 0.77, 0.50, 0.46>)  #local N2=npoints-1;
    addpoint(< 0.77,-0.50, 0.46>)  #local N3=npoints-1;
    addpoint(< 0.00,-1.12, 0.33>)  #local N4=npoints-1;
    addpoint(<-0.77,-0.50, 0.46>)  #local N5=npoints-1;
    addpoint(<-0.77, 0.50, 0.46>)  #local N6=npoints-1;
    addpoint(< 0.50, 0.77,-0.46>)  #local S1=npoints-1;
    addpoint(< 1.12, 0.00,-0.33>)  #local S2=npoints-1;
    addpoint(< 0.50,-0.77,-0.46>)  #local S3=npoints-1;
    addpoint(<-0.50,-0.77,-0.46>)  #local S4=npoints-1;
    addpoint(<-1.12, 0.00,-0.33>)  #local S5=npoints-1;
    addpoint(<-0.50, 0.77,-0.46>)  #local S6=npoints-1;
    addpoint(< 0.50, 0.00,-1.10>)  #local SS1=npoints-1;
    addpoint(<-0.50, 0.00,-1.10>)  #local SS2=npoints-1;
    make_lune(N1,N2,N3,N4,NN2,NN1)
    make_lune(N4,N5,N6,N1,NN1,NN2)
    make_lune(S2,SS1,SS2,S5,S6,S1)
    make_lune(S2,S3,S4,S5,SS2,SS1)
    make_triangle(N1,S1,N2)
    make_triangle(N2,S2,N3)
    make_triangle(N3,S3,N4)
    make_triangle(N4,S4,N5)
    make_triangle(N5,S5,N6)
    make_triangle(N6,S6,N1)
    optimise(400,0.000001)
    showvtxs()
    autobalance()
    autoface()
#                                                                                                            end

#                                                                                                            macro bilunabirotunda() // J91
    // start with icosahedron
  addevenpermssgn(<0,1,tau>,<0,1,1>)
  //   showvtxs()
  // trim back to 8 vertices
  drop_halfspace(<-1,-tau,0>,-tau)
  drop_halfspace(<-1,tau,0>,-tau)
  drop_halfspace(<1,0,0>,-1)
  // now shift all vertices into halfspace x >= 0, and mirror
#                                                                                                            local i = 0; #                                                                                                            local minx = 999;
#                                                                                                            while (i < npoints)
#                                                                                                            local minx = min(minx, points[i].x);
    //#if (minx>points[i].x) #local minx=points[i].x; #end
#                                                                                                            local i    = i + 1;
#                                                                                                            end // (while loop)
#                                                                                                            local i = 0; #                                                                                                            local np = npoints;
#                                                                                                            while (i < np)
#                                                                                                            declare points[i] = points[i] + < -minx, 0, 0 > ;
#                                                                                                            if (points[i].x > 0) addpoint(<-points[i].x, points[i].y, points[i].z>) #                                                                                                            end
#                                                                                                                local i = i + 1;
#                                                                                                                end // (while loop)
  autoface()
#                                                                                                                end

#                                                                                                                macro triangular_hebesphenorotunda() // J91
    // Coords found by taking 7 vtxs of an icosahedron, placing one vtx
    // at origin, which is centre of the one hexagonal face.
    addevenperms( <1,tau,0>-<tau,0,1>)
    addevenperms( <0,1,tau>-<tau,0,1>)
    addevenperms( <-1,tau,0>-<tau,0,1>)
    addevenperms( <-tau,0,1>-<tau,0,1>)
    addevenperms( <0,1,-tau>-<tau,0,1>)
    addevenperms(-<1,tau,0>-<tau,0,1>)
    autobalance()
    autoface()
#                                                                                                                end

//<<<<<<<<<<<<<<<<< added AGK  [20041101]



This_shape_will_be_drawn()

//Random rotations are (hopefully) equally distributed...
#                                                                                                                declare rot1 = rand(rotation) * pi * 2;
#                                                                                                                declare rot2 = acos(1 - 2 * rand(rotation));
#                                                                                                                declare rot3 = (rand(rotation) + clock) * pi * 2;
#                                                                                                                macro dorot()
  rotate rot1*180/pi*y
  rotate rot2*180/pi*x
  rotate rot3*180/pi*y
#                                                                                                                end

// drawit()

#                                                                                                                if (1)
    //Scale shape to fit in unit sphere
#                                                                                                                    local b = 0;
#                                                                                                                    local a = 0; #                                                                                                                    while (a < npoints - .5)
#                                                                                                                    local c = vlength(points[a]); #                                                                                                                    if (c > b) #                                                                                                                    local b = c; #                                                                                                                    end
#                                                                                                                    local a                                                                                                                                                                                                                                                                                    = a + 1; #                                                                                                                    end
#                                                                                                                    local a                                                                                                                                                                                                                                                                                    = 0; #                                                                                                                    while (a < npoints - .5)
#                                                                                                                    local points[a]                                                                                                                                                                                                                                                                            = points[a] / b;
#                                                                                                                    local a                                                                                                                                                                                                                                                                                    = a + 1; #                                                                                                                    end
#                                                                                                                    local a                                                                                                                                                                                                                                                                                    = 0; #                                                                                                                    while (a < nfaces - .5)
#                                                                                                                    local faces[a]                                                                                                                                                                                                                                                                             = faces[a] * b;
#                                                                                                                    local a                                                                                                                                                                                                                                                                                    = a + 1; #                                                                                                                    end
#                                                                                                                    end

//Draw edges
#                                                                                                                    macro addp(a)
#                                                                                                                    declare p[np] = a;
#                                                                                                                    declare np    = np + 1;
#                                                                                                                    end
#                                                                                                                    local a    = 0; #                                                                                                                    while (a < nfaces - .5)
#                                                                                                                    declare p  = array[20];
#                                                                                                                    declare np = 0;
#                                                                                                                    local b    = 0; #                                                                                                                    while (b < npoints - .5)
#                                                                                                                    if (vdot(faces[a], points[b]) > 1 - 0.00001) addp(b) #                                                                                                                    end
#                                                                                                                        local b                                                                                                                              = b + 1; #                                                                                                                        end
#                                                                                                                        local c                                                                                                                              = 0; #                                                                                                                        while (c < np - .5)
#                                                                                                                        local d                                                                                                                              = 0; #                                                                                                                        while (d < np - .5) #                                                                                                                        if (p[c] < p[d] - .5)
#                                                                                                                        local f                                                                                                                              = 1;
#                                                                                                                        local e                                                                                                                              = 0; #                                                                                                                        while (e < np - .5) #                                                                                                                        if (e != c & e != d & vdot(vcross(points[p[c]], points[p[d]]), points[p[e]]) < 0)
#                                                                                                                        local f                                                                                                                              = 0;
#                                                                                                                        end #                                                                                                                        local e = e + 1; #                                                                                                                        end
#                                                                                                                        if (f)
        object {
          cylinder { points[p[c]], points[p[d]], .01 dorot() }
          pigment { colour <.3,.3,.3> }
          finish { ambient 0 diffuse 1 phong 1 }
        }
#                                                                                                                            end #                                                                                                                            end
#                                                                                                                            local d = d + 1; #                                                                                                                            end
#                                                                                                                            local c = c + 1; #                                                                                                                            end
#                                                                                                                            local a = a + 1; #                                                                                                                            end
/*#local a=0; #while(a<npoints-.5)
  #local b=a+1; #while(b<npoints-.5)
    #if(vlength(points[a]-points[b])<elength+0.00001)
      object {
        cylinder { points[a], points[b], .01 dorot() }
        pigment { colour <.3,.3,.3> }
        finish { ambient 0 diffuse 1 phong 1 }
      }
    #end
  #local b=b+1; #end
#local a=a+1; #end*/

//>>>>>>>>>>>>>>>>> changed AGK  [20041101]
//Draw points
#                                                                                                                            if (1)
#                                                                                                                                local a = 0; #                                                                                                                                while (a < npoints - .5)
      object {
        sphere { points[a], .01 dorot() }
        pigment { colour <.3,.3,.3> }
        finish { ambient 0 diffuse 1 phong 1 }
      }
#                                                                                                                                local a = a + 1; #                                                                                                                                end
#                                                                                                                            else
#                                                                                                                                local a = 0; #                                                                                                                                while (a < npoints - .5)
#                                                                                                                                local i = 0;
#                                                                                                                                while (i <= a)
          object {
            sphere { (1+i/10)*points[a], .01 dorot() }
            pigment { colour <.3,.3,.3> }
            finish { ambient 0 diffuse 1 phong 1 }
          }
#                                                                                                                                local i = i + 1;
#                                                                                                                                end
#                                                                                                                                local a = a + 1; #                                                                                                                                end
#                                                                                                                                end
//<<<<<<<<<<<<<<<<< changed AGK  [20041101]

#                                                                                                                                if (notwireframe)
//Draw planes
object {
  intersection {
#                                                                                                                                    local a = 0; #                                                                                                                                    while (a < nfaces - .5)
      plane { faces[a], 1/vlength(faces[a]) }
#                                                                                                                                    local a = a + 1; #                                                                                                                                    end
    //planes()
    //sphere { <0,0,0>, 1 }
    //sphere { <0,0,0>, ld+.01 inverse }
    dorot()
  }
  pigment { colour rgbt <.8,.8,.8,.4> }
  finish { ambient 0 diffuse 1 phong flashiness #if(withreflection) reflection { .2 } #end }
  //interior { ior 1.5 }
  photons {
    target on
    refraction on
    reflection on
    collect on
  }
}
#                                                                                                                                    end

//  CCC Y Y PP
//  C   Y Y P P
//  C    Y  PP
//  C    Y  P
//  CCC  Y  P

#                                                                                                                                    local a = 0;
#                                                                                                                                    while (a < 11.0001)
  light_source { <4*sin(a*pi*2/11), 5*cos(a*pi*6/11), -4*cos(a*pi*2/11)> colour (1+<sin(a*pi*2/11),sin(a*pi*2/11+pi*2/3),sin(a*pi*2/11+pi*4/3)>)*2/11 }
//  light_source { <4*sin(a*pi*2/11), 5*cos(a*pi*6/11), -4*cos(a*pi*2/11)> 
 //        colour (1+<sin(a*pi*2/11),sin(a*pi*2/11+pi*2/3),sin(a*pi*2/11+pi*4/3)>)*2/11 }
#                                                                                                                                    local a = a + 1;
#                                                                                                                                    end

background { color <1,1,1> }

#                                                                                                                                    if (0) // default framing vs auto-framing
    camera {
      perspective
      location <0,0,0>
      direction <0,0,1>
      right x/2
      up y/2
      sky <0,1,0>
      location <0,0,-4.8>
    //  location <0,0,-10>
      look_at <0,0,0>
    }
#                                                                                                                                    else
    // some auto-framing.  Not for animated versions.
#                                                                                                                                        declare camera_loc    = < 0, 0, -4.8 > ;
#                                                                                                                                        declare max_elevation = 0; #                                                                                                                                        declare max_bearing = 0;
#                                                                                                                                        declare i                                                                                                                                                                               = 0;
#                                                                                                                                        while (i < npoints)
#                                                                                                                                        declare sighting      = points[i];
#                                                                                                                                        declare sighting      = vaxis_rotate(sighting, y, rot1 * 180 / pi);
#                                                                                                                                        declare sighting      = vaxis_rotate(sighting, x, rot2 * 180 / pi);
#                                                                                                                                        declare sighting      = vaxis_rotate(sighting, y, rot3 * 180 / pi);
#                                                                                                                                        declare sighting      = sighting - camera_loc;
#                                                                                                                                        declare elevation     = sighting.y / sighting.z;
#                                                                                                                                        declare bearing       = sighting.x / sighting.z;
#                                                                                                                                        declare max_elevation = max(max_elevation, abs(elevation));
#                                                                                                                                        declare max_bearing   = max(max_bearing, abs(bearing));
    //     sphere{ (camera_loc+<bearing,elevation,1>*10), 0.2 
    //         pigment{ colour rgb <1,0,1> } }
#                                                                                                                                        declare i             = i + 1;
#                                                                                                                                        end
#                                                                                                                                        debug concat("Maximum: Elevation = ", str(max_elevation, 4, 4), "  Bearing = ", str(max_bearing, 4, 4), "\n")
#                                                                                                                                        if (1) // 1:1 aspect ratio
#                                                                                                                                            declare max_bearing = max(max_elevation, max_bearing); #                                                                                                                                            declare max_elevation = max_bearing;
#                                                                                                                                            end
#                                                                                                                                            if (1) // 5% border
#                                                                                                                                                declare max_bearing   = 1.05 * max_bearing;
#                                                                                                                                                declare max_elevation = 1.05 * max_elevation;
#                                                                                                                                                end
    camera {
      perspective
      location camera_loc
      direction <0,0,.5>
      right x*max_bearing
      up y*max_elevation
    }
#                                                                                                                                                end
global_settings {
  max_trace_level 40
  photons {
    count 200000
    autostop 0
  }
}

};

} } }

#                                                                                                                                            endif
