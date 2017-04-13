//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Cleaver - A MultiMaterial Conforming Tetrahedral Meshing Library
//
// -- TetMesh Class
//
// Primary Author: Josh Levine (jlevine@sci.utah.edu)
// Secondary Author: Jonathan Bronson (bronson@sci.utah.ed)
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
//  Copyright (C) 2011, 2012, Jonathan Bronson
//  Scientific Computing & Imaging Institute
//  University of Utah
//
//  Permission is  hereby  granted, free  of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files  ( the "Software" ),  to  deal in  the  Software without
//  restriction, including  without limitation the rights to  use,
//  copy, modify,  merge, publish, distribute, sublicense,  and/or
//  sell copies of the Software, and to permit persons to whom the
//  Software is  furnished  to do  so,  subject  to  the following
//  conditions:
//
//  The above  copyright notice  and  this permission notice shall
//  be included  in  all copies  or  substantial  portions  of the
//  Software.
//
//  THE SOFTWARE IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF ANY
//  KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT  LIMITED  TO THE
//  WARRANTIES   OF  MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT  SHALL THE AUTHORS OR
//  COPYRIGHT HOLDERS  BE  LIABLE FOR  ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
//  USE OR OTHER DEALINGS IN THE SOFTWARE.
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#ifndef TETMESH_H
#define TETMESH_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Vertex.h"
#include "HalfEdge.h"
#include "HalfFace.h"
#include "Tet.h"
#include "BoundingBox.h"

#define VERTS_PER_FACE 3
#define EDGES_PER_FACE 3
#define TETS_PER_FACE 2

#define VERTS_PER_TET 4
#define EDGES_PER_TET 6
#define FACES_PER_TET 4

namespace cleaver
{
//              BCCLattice   BCCOctree
enum MeshType { Regular,    Structured };
enum MeshFormat { Tetgen, Scirun, Matlab, VtkUSG, VtkPoly, PLY };

class TetMesh
{
public:

    TetMesh();
    TetMesh(BoundingBox b);
    TetMesh(const std::vector<Vertex*> &verts, const std::vector<Tet*> &tets);
    ~TetMesh();

    size_t fixVertexWindup(bool verbose);

    Tet* createTet(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *v4, int material);
    void removeTet(int t);
    std::vector<Tet*>::iterator removeTet(std::vector<Tet*>::iterator);

    //void writeOff(const std::string &filename);
    void writeMesh(const std::string &filename, MeshFormat format, bool verbose = false);
    void writeVtkPolyData(const std::string &filename, bool verbose = false);
    void writeVtkUnstructuredGrid(const std::string &filename, bool verbose = false);
    void writeMatlab(const std::string &filename, bool verbose = false);   // matlab format
    void writeNodeEle(const std::string &filename, bool verbose = false, bool includeMaterials = true, bool includeParent = false);  // tetview format
    void writePtsEle(const std::string &filename, bool verbose = false);   // scirun format
    //void writePlyConnected(const std::string  &filename);
    void writeStencilPly(const std::string &filename, bool verbose = false);
    void writePly(const std::string &filename, bool verbose = false);
    void writeMultiplePly(const std::vector<std::string> &inputs, const std::string &filename, bool verbose = false);
    void writeInfo(const std::string &filename, bool verbose = false);

    bool isValid();
    void stripMaterial(char material, bool verbose=false);

    void constructNewAdjacency();
    void constructBottomUpIncidences(bool verbose=false);

    void constructEdges();
    void constructFaces();
    void computeAngles();
    void computeDihedralHistograms();
    void computeBounds();
    void updateBounds(Vertex *vertex);

    void removeExternalTets();   // external tets are tets with 4 'external' vertices.
    void removeLockedTets();     // locked tets are tets which cannot move/change due to cuts/warps/snaps.
    void removeMaterial(int m);  // remove all tets of material i
    void removeOutsideBox(BoundingBox &box);

    std::vector<Vertex*> verts;           // these arrays are probably cache-inefficient
    std::vector<Tet*> tets;               // these arrays are probably cache-inefficient   // consider testing with non-pointer based arrays
    std::vector<Face*> faces;             // these arrays are probably cache-inefficient


    static TetMesh* createFromNodeElePair(const std::string &nodeFileName, const std::string &eleFileName, bool verbose = false);

    std::vector<HalfFace> halfFaces;
    std::map<std::pair<int, int>, HalfEdge*> halfEdges;
    HalfEdge* halfEdgeForVerts(Vertex *v1, Vertex *v2);

    bool imported;
    double min_angle;      // smallest dihedral angle
    double max_angle;      // largest dihedral angle
    double time;           // time taken to mesh
    int material_count;
    BoundingBox bounds;
    std::string name;

public:
    // Adjacency Queries
    static void getAdjacencyListsForFace(HalfFace *face, Vertex *verts[3], HalfEdge *edges[3]);
    void getAdjacencyListsForTet(Tet *tet, Vertex *verts[4], HalfEdge *edges[6], HalfFace *faces[4]);
    void getRightHandedVertexList(Tet *tet, Vertex *verts[15]);

    std::vector<HalfEdge*> edgesAroundVertex(Vertex *v);
    std::vector<HalfFace*> facesAroundVertex(Vertex *v);
    std::vector<Tet*>      tetsAroundVertex(Vertex *v);
    std::vector<HalfFace*> facesAroundEdge(HalfEdge *e);
    std::vector<Tet*>      tetsAroundEdge(HalfEdge *e);
    std::vector<Tet*>      tetsAroundFace(HalfFace *f);
    std::vector<Vertex*>   vertsAroundFace(HalfFace *f);
    std::vector<Vertex*>   vertsAroundTet(Tet *t);
    std::vector<HalfFace*> facesAroundTet(Tet *t);
    std::vector<HalfEdge*> edgesAroundTet(Tet *t);

    std::vector<HalfFace*> facesIncidentToBothTetAndEdge(Tet *tet, HalfEdge *edge);

    Tet* oppositeTetAcrossFace(Tet *tet, HalfFace *face);
};

} // namespace cleaver

#endif // TETMESH_H

