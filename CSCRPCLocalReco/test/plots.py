import ROOT
# Create a new canvas, and customize it.
c1 = ROOT.TCanvas( 'c1', 'CSC Segments + RPC Hits', 200, 10, 700, 700 )
c1.Divide(2,1)
fname = "cscrpc.root"

f = ROOT.TFile.Open(fname)
tree = f.Get("dumpcsc/cscrpc")

#vec = ROOT.std.vector('short')()
#tree.SetBranchAddress('RPCGlobalPosition_X', vec)

#Histograms
#hist = ROOT.TH1D("hRPCGlobalPositionXY","hist title",100,0,900)
c1.cd(1)
tree.Draw("RPCGlobalPosition_X:RPCGlobalPosition_Y >> hRPCXYrpcIDSector2(100,-900,900,100,-900,900)","(rpcIDRegion != 0 && (rpcIDRing==2 || rpcIDRing==3))","COLZ")
#c1.cd(2)
#tree.Draw("RPCGlobalPosition_X:RPCGlobalPosition_Y >> hRPCXYrpcIDSector3(100,-900,900,100,-900,900)","rpcIDSector==3","COLZ")

#c1.cd(3)
#tree.Draw("CSCGlobalPosition_X:CSCGlobalPosition_Y >> hCSCXYcscIDStation2(100,-900,900,100,-900,900)","cscIDStation==2","COLZ")
c1.cd(2)
tree.Draw("CSCGlobalPosition_X:CSCGlobalPosition_Y >> hCSCXYcscIDStation3(100,-900,900,100,-900,900)","cscIDRing==2","COLZ")



## wait for input to keep the GUI (which lives on a ROOT event dispatcher) alive
if __name__ == '__main__':
   rep = ''
   while not rep in [ 'q', 'Q' ]:
      rep = raw_input( 'enter "q" to quit: ' )
      if 1 < len(rep):
         rep = rep[0]








#n = 0
'''
for i in range(tree.GetEntries()):
    tree.GetEntry(i)
#    print(vec.size())
    hist.Fill(vec.size())


hist.Draw()
c1.Modified()
c1.Update()


for evt in tree:
    x = evt.RPCGlobalPosition_X
    y = evt.RPCGlobalPosition_Y
    z = evt.RPCGlobalPosition_Z

    print("x={} y={} z={}".format(x,y,z))
    n += 1
    if n > 1000:
        break
    pass
'''
