#include<bits/stdc++.h>
using namespace std;

vector<int> hc;

int find_redundant_length(int len){
	int pro = 1;
	int it = 0;
	
	while(pro <= (len + it) ){
		pro *= 2;
		it++;
	}
	
	return len+it;
	
}

void fill_redbits(string msg,int total){
	
	int pro = 1;
	int idx = 0;
	for(int i = 1;i<=total;i++){
		if(i == pro){
			pro*=2;
			continue;
		}
		
		hc[i] = msg[idx] - '0';
		++idx;
	}
	
	pro = 1;
	int length = msg.length();
	int it = 0;
	
	while(pro < total){
		int count = 0;
		cout<<it<<endl;
		for(int i=1;i<=total;i++){
			if((i>>it)%2){
				cout<<i<<" ";
				if(hc[i] == 1){
					++count;
				}
			}
		}		
		cout<<count<<endl;
		if(count%2) hc[pro] = 1;
		else hc[pro] = 0;
		++it;
		pro*=2;
	}
	
}

int main(){
	string msg;
	
	cin>>msg;
	int len =msg.length();
	
	int total_length = find_redundant_length(len);
	
	cout<<"no of total bits : "<<total_length<<endl;
	
	hc = vector<int>(total_length+1);
	
	fill_redbits(msg,total_length);
	
	cout<<"Hamming code is : "<<endl;
	for(int i=1;i<=total_length;i++){
		cout<<hc[i];
	}
	cout<<endl;
	
	return 0;
}
