#ifndef DXPHYSPOINT
#define DXPHYSPOINT

class DXFRAME_API dxPhysPoint {

protected:

	D3DXVECTOR2 P;
	D3DXVECTOR2 V;
	D3DXVECTOR2 A;

public:
	
	dxPhysPoint ();
	~dxPhysPoint ();

	virtual void Reset ();

	virtual void Update (float dt);

	virtual void SetPos (D3DXVECTOR2 v);
	virtual D3DXVECTOR2 GetPos ();

	virtual void SetVel (D3DXVECTOR2 v);
	virtual D3DXVECTOR2 GetVel ();

	virtual void SetAcl (D3DXVECTOR2 v);
	virtual D3DXVECTOR2 GetAcl ();
};

#endif