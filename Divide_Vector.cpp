vector<vector<string>> divide_vector(const vector<string> vec){
    int vec_len=vec.size();
    int base=vec.size()/3;
    vector<string> v1,v2,v3;
    for(int i=0;i<base;i++){
        v1.push_back(vec[i]);
        v2.push_back(vec[i+base]);
        v3.push_back(vec[i+base*2]);
    }
    if(vec_len%3==1){
        v3.push_back(vec.back());
    }
    if(vec_len%3==2){
        v2.push_back(v3[0]);
        v3.erase(v3.begin());
        v3.push_back(vec[vec_len-2]);
        v3.push_back(vec[vec_len-1]);
    }
    vector<vector<string>> vecc(3);
    vecc[0]=v1;
    vecc[1]=v2;
    vecc[2]=v3;
    return vecc;
}
