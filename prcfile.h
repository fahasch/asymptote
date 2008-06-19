#include "prc/oPRCFile.h"

namespace camp {

class prcfile : public oPRCFile {
  std::list<PRCentity *> entities;
public:  
  prcfile(string name) : oPRCFile(name.c_str()) {}
  ~prcfile() {

    for(std::list<PRCentity *>::iterator p=entities.begin();
	p != entities.end(); ++p) {
      assert(*p);
      delete *p;
    }
  }

  void add(PRCentity* e) {
    entities.push_back(e);
    oPRCFile::add(e);
  }
};
  
inline void writeBezierKnots(PRCbitStream &out, uint32_t d, uint32_t n)
{
  out << (double) 1;
  for(uint32_t i=1; i < d+n; ++i)
    out << (double) ((i+2)/d); // integer division is intentional
  out << (double) ((d+n+1)/d);
}
    
class PRCBezierCurve : public PRCcurve
{
  uint32_t d;
  uint32_t n;
public:
  PRCBezierCurve(oPRCFile *p, uint32_t d, uint32_t n, double cP[][3],
		 const RGBAColour &c) :
    PRCcurve(p,d,n,cP,NULL,c,false,NULL), d(d), n(n) {}
private:
  void writeKnots(PRCbitStream &out) {
    writeBezierKnots(out,d,n);
  }
};

class PRCBezierSurface : public PRCsurface
{
  uint32_t dU,dV;
  uint32_t nU,nV;
public:
  PRCBezierSurface(oPRCFile *p, uint32_t dU, uint32_t dV, uint32_t nU,
		   uint32_t nV, double cP[][3], const RGBAColour &c) :
    PRCsurface(p,dU,dV,nU,nV,cP,NULL,NULL,c,false,NULL), dU(dU), dV(dV),
    nU(nU), nV(nV) {}
private:
  void writeKnots(PRCbitStream &out) {
    writeBezierKnots(out,dU,nU);
    writeBezierKnots(out,dV,nV);
  }
};

}