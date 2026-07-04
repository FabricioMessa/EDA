import hnswlib, numpy as np, time
N,DIM,CYCLES,CHUNK=10000,100,100,50
SEEDS=[42,123,456]
print("MN-RU ADAPTATIVO")
for seed in SEEDS:
    np.random.seed(seed)
    p=hnswlib.Index(space="l2",dim=DIM)
    p.init_index(max_elements=N*3,M=16,ef_construction=200,random_seed=seed,allow_replace_deleted=True)
    p.set_ef(100); p.set_tau(2000); p.set_tau_params(10.0,5.0,200)
    data=np.random.randn(N,DIM).astype(np.float32)
    p.add_items(data,np.arange(N))
    cur=set(range(N)); nl=N
    for _ in range(CYCLES):
        a=np.array(list(cur)); td=np.random.choice(a,min(CHUNK,len(a)),replace=False)
        for d in td: p.mark_deleted(int(d)); cur.discard(d)
        nd=np.random.randn(len(td),DIM).astype(np.float32)
        p.add_items(nd,np.arange(nl,nl+len(td)),replace_deleted=True)
        cur.update(range(nl,nl+len(td))); nl+=len(td)
    qids=np.arange(900000,900000+200); qd=np.random.randn(200,DIM).astype(np.float32)
    p.add_items(qd,qids)
    t0=time.time()
    hits=sum(1 for i in range(200) if p.knn_query(qd[i],k=1)[0][0]==qids[i])
    lat=(time.time()-t0)/200*1000
    unc=p.get_unreachable_count(); ops=p.get_operation_counter() 
    tau=p.get_tau(); bkp=p.get_backup_count() if p.has_backup_index() else 0
    print(f"Seed {seed}: Recall={hits/200:.4f} Inalc={unc} Tau={tau} Bkp={bkp} Lat={lat:.4f}ms")
