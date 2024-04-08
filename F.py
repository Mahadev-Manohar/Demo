import math
#give your values
full_datagram=1500
full_mtu=296
header=20

datagram=full_datagram-header
mtu=full_mtu-header

no_frag = datagram/mtu
no_fragments=math.ceil(no_frag)
print("No of fragments generated: ",no_fragments)

fragments=[0]*no_fragments
mf_bit_list=[0]*no_fragments
frag_offset_list=[0]*no_fragments



for i in range(0,no_fragments-1):
    fragments[i]=full_mtu
    #print(fragments[i])
    mf_bit_list[i]=1
    
    
for i in range(0,no_fragments):
    if i==0:
        frag_offset_list[i]=0
    else:
        frag_offset_list[i]=math.floor((mtu/8))*i

temp = mtu*(no_fragments-1)
temp1=datagram-temp
#print(temp1)
fragments[no_fragments-1]=temp1+header
mf_bit_list[no_fragments-1]=0

tot_length = frag_offset_list[no_fragments-1]*8


print(fragments)
print(mf_bit_list)
print(frag_offset_list)
print("total length: ",tot_length)