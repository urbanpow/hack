int N;
vector<int> v(N), Tree(4*N), Lazy(4*N, 0);

void buildTree(int tidx, int lo, int hi){

	if(lo==hi){
		Tree[tidx] = v[hi];
		return;
	}

	int mid = (lo+hi)/2;

	buildTree(2*tidx+1, lo, mid);
	buildTree(2*tidx+2, mid+1, hi);

	Tree[tidx] = Tree[2*tidx+1] + Tree[2*tidx+2];

}

void updateTree(int tidx, int lo, int hi, int idx, int val){

	if(lo==hi){
		Tree[tidx] = val;
		return;
	}

	int mid = (lo+hi)/2;

	if(idx<=mid)
		updateTree(2*tidx+1, lo, mid, idx, val);
	else
		updateTree(2*tidx+2, mid+1, hi, idx, val);

	Tree[tidx] = Tree[2*tidx+1] + Tree[2*tidx+2];

}

int query(int tidx, int lo, int hi, int L, int R){

	// The Range which we are currently in : [lo, hi]
	// The Range of which we have to compute the answer : [L, R]

	// Outside Range
	if( R<lo || L>hi )
		return 0;

	// In Range
	if( lo>=L && hi<=R )
		return Tree[tidx];

	// Partial Overlap
	int mid = (lo+hi)/2;
	int leftans = query(2*tidx+1, lo, mid, L, R);
	int rightans = query(2*tidx+2, mid+1, hi, L, R);

	return leftans + rightans;

}
