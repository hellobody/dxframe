#ifndef DXPHYSPOINT
#define DXPHYSPOINT

class dxPhysPoint {

protected:

	D3DXVECTOR2 P;
	D3DXVECTOR2 V;

public:
	
	dxPhysPoint ();
	~dxPhysPoint ();

	void Reset ();

	void SetPos (D3DXVECTOR2 v);
	D3DXVECTOR2 GetPos ();

	void SetVel (D3DXVECTOR2 v);
	D3DXVECTOR2 GetVel ();
};

#endif