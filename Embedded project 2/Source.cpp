#include <iostream>
#include <string.h>

using namespace std;

class node
{
public:
	char val[32];
	float freq;
	node * next;
	node(char * v)
	{
		strcpy(val,v);
		//cout<<"v = "<<v<<" val = "<<val<<"\n";
		//val[32] = '\0';
		freq = 1;
		next = NULL;
	}
};


class freqlist
{
public:
	node * start;
	freqlist()
	{
		start = NULL;
	}
	void add(char * toadd)
	{
		int flag=0;
		node * temp;
		temp = start;
		if(temp == NULL)
		{
			//cout<<"\nsee this = "<<toadd<<"\n";
			node* temp1 = new node(toadd);
			//temp1->freq = 1;
			temp1->val[32] = '\0';
		//	cout<<"temp = "<<temp->val[32]<<"\n";
			cout<<"freq = "<<temp1->freq<"\n";
			start = temp1;
			cout<<"freq = "<<temp1->freq<<" "<<start->freq<<"\n";
			flag = 1;
		}
		else if(temp->next == NULL)
		{
			if(!strcmp(temp->val,toadd))
			{
				temp->freq++;
			}
			else
			{
				temp = new node(toadd);
				temp->val[32] = '\0';

			//	temp->freq++;
				start->next = temp;
			}
		}
		else
		{
			while(temp->next != NULL)
			{
				if(!strcmp(temp->val,toadd))
				{
					temp->freq++;
					flag = 1;
					break;
				}
				temp = temp->next;
			}
			if(flag == 0)
			{
				if(!strcmp(temp->val,toadd))
				{
					temp->freq++;
					flag = 1;
				}
				else
				{
					node * newnode = new node(toadd);
					newnode->val[32] = '\0';

					temp->next = newnode;
				//	newnode->freq++;
				}
			}
		}
	}
	void printlist()
	{
		node * temp = start;
		while(temp!=NULL)
		{
			cout << temp->val<<" freq = "<< temp->freq<<"\n";
			temp = temp->next;
		}

	}
	node ** getmax4()
	{
		if(start == NULL)
		{	
			return NULL;
		}
		else 
		{
			node ** ret = new node*[4];
			node usefullnode("0");
			usefullnode.freq = 0;
			node * max1 = &usefullnode;
			node * max2 = &usefullnode;
			node * max3 = &usefullnode;
			node * max4 = &usefullnode;
			node * temp = start;
			while(temp != NULL)
			{
				
				if(temp->freq > max1->freq)
					max1 = temp;
				temp = temp->next;
			}
			temp = start;
			while(temp != NULL)
			{
				if((temp != max1) && (temp->freq > max2->freq))
					max2 = temp;
				temp = temp->next;
			}
			temp = start;
			while(temp != NULL)
			{
				if((temp != max1) && (temp != max2) && (temp->freq > max3->freq))
					max3 = temp;
				temp = temp->next;
			}
			temp = start;
			while(temp != NULL)
			{
				if((temp != max1) && (temp != max2) && (temp != max3) && (temp->freq > max4->freq))
					max4 = temp;
				temp = temp->next;
			}
		//	cout<<"max 1 = "<<max1->freq<<"-"<<max1->val<<" max 2 = "<<max2->freq<<" max 3 = "<<max3->freq<<" max 4 = "<<max4->freq;
			ret[0] = max1;
			ret[1] = max2;
			ret[2] = max3;
			ret[3] = max4;
			return ret;
		}
		
	}


};

class dictionarynode
{
public:
	char * index;
	char * val;
	float freq;
	dictionarynode(char * i, char * v, float f)
	{
		index = new char[2];
		val = new char[32];
		strcpy(index,i);
		strcpy(val, v);
		freq = f;
	}

};

class dictionary
{
public:
	dictionarynode **entry;
	dictionary(node ** max)
	{
		entry = new dictionarynode * [3];
		entry[0] = new dictionarynode("00",max[0]->val,max[0]->freq);
		entry[1] = new dictionarynode("01",max[1]->val,max[1]->freq);
		entry[2] = new dictionarynode("10",max[2]->val,max[2]->freq);
		entry[3] = new dictionarynode("11",max[3]->val,max[3]->freq);
	}

};



char * getaline(FILE * fp);
char * compress(char * inp, dictionary *dic);
char * rlecompress(char *inp,dictionary *dic,int ctr);
int xorcount(char *inp, dictionarynode * d);
int xorloc(char *inp,dictionarynode *d,int i);
char * bin(int i);
char * bin3(int i);
char * flip(char *in, char * pos);
char * flip2(char *in, char * pos);
char * flip3(char *in, char * pos);
char * flip4(char *in, char * pos);
int btod5(char * p);
int btod3(char * p);

int main()
{
	char *a, *temp,*temp2,*temp3,*writetofile,*index, *value, tempchar;
	char *out = new char[32];
	int count=0, ctr = 0, tempint;
	dictionary *dic, *d;
	dictionarynode *d1,*d2,*d3,*d4;
	a = new char[10];
	freqlist *freq = new freqlist;
	node ** max;
	FILE *file,*file2;
	file = fopen("A:\original.txt","r");
	temp = getaline(file);
	while(temp != NULL)
	{
		count++;
		freq->add(temp);
//		cout<<temp<<"\n";
		temp = getaline(file);
	//	freq->add(temp);
	}
	fclose(file);
//	cout<<"COUNT = "<<count*32<<"\n";
	freq->printlist();
	max = freq->getmax4();
	//cout<<"max 4 values are as : "<<max[0]->val<<" "<<max[1]->val<<" "<<max[2]->val<<" "<<max[3]->val<<"\n";
	dic = new dictionary(max);
	file = fopen("A:\original.txt","r");
	file2 = fopen("A:\compressed.txt","r");
	temp = getaline(file);
	while(temp != NULL)
	{
		temp2 = getaline(file);
		if(temp2!= NULL && !strcmp(temp,temp2))		//2 matches
		{
			temp3 = getaline(file);
			if(temp3 != NULL && !strcmp(temp,temp3))		//3 matches
			{
				ctr = 3;
				temp2 = getaline(file);
				while(ctr<9 && temp2!=NULL && !strcmp(temp2,temp))		//while matches are found and are less than 8
				{
					ctr++;
					temp2 = getaline(file);
				}
				//cout<<"RLE compress "<<ctr<<"\n";
				
				cout<<compress(temp,dic)<<"\n"<<rlecompress(temp,dic,ctr)<<"\n";
				//cout<<temp <<"->"<<compress(temp,dic)<<" "<<rlecompress(temp,dic,ctr)<<"\n";
				temp = temp2;
			}
			else
			{
				cout<<compress(temp,dic)<<"\n";
				cout<<compress(temp2,dic)<<"\n";
				if(temp3 == NULL)
					break;
				else
					temp = temp3;
				
			}
			
		}
		else
		{
			cout<<compress(temp,dic)<<"\n";
			if(temp2 == NULL)
				break;
			temp = temp2;
			
		}

		//cout<<temp <<" -> "<<compress(temp,dic)<<"\n";
		//writetofile = compress(temp,dic);
		//fprintf(file2,"%s",writetofile);
		//temp = getaline(file);
	}
	//fprintf(file2,"%d",1223);
	//cout<<bin(30);

	// READ DICTIONARY ENTRIES

	while(fgetc(file2) != '2');
	while (fgetc(file2) != '\n');
	d1 = new dictionarynode("00", getaline(file2),3);
	d2 = new dictionarynode("01", getaline(file2),2);
	d3 = new dictionarynode("10", getaline(file2),1);
	d4 = new dictionarynode("11", getaline(file2),0);
	fclose(file2);
	file2 = fopen("A:\compressed.txt","r");
	// DECOMPRESSION
	do
	{
		
		fwrite(
		index = new char [3];
		index[0] = fgetc(file2);
		if(index[0] == '\n')
			index[0] = fgetc(file2);
		else if(index[0] == '2')
			break;
		index[1] = fgetc(file2);
		if(index[1] == '\n')
			index[1] = fgetc(file2);
		index[2] = fgetc(file2);
		if(index[2] == '\n')
			index[2] = fgetc(file2);
		index[3] = '\0';
//		cout<<index<<" - ";
		if(!strcmp(index,"000"))
		{
			int i =0, flag = 0;
			while(i<32)
			{
				out[i] = fgetc(file2);
				if(out[i] == '\n')
				{	out[i] = fgetc(file2);
					{	if(out[i] == '2')
						{
							flag = 1;
							break;
						}
					}
				}		//included case for last zeros.
				i++;
			}
			out[32] = '\0';
			if(!flag)
				cout<<out<<"\n";

		}
		else if(!strcmp(index,"001"))
		{
			char *ind = new char [2]; 
			int i =0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[2] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"010"))
		{
			char * pos = new char [5];
			char * ind = new char [2];
			int i =0;
			while(i<5)
			{
				pos[i] = fgetc(file2);
				if(pos[i] == '\n')
					pos[i] = fgetc(file2);
				i++;
			}
			pos[i] = '\0';
			i = 0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[i] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			out = flip(out,pos);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"011"))
		{
			char * pos1 = new char [5];
			char * pos2 = new char [5];
			char * ind = new char [2];
			int i =0;
			while(i<5)
			{
				pos1[i] = fgetc(file2);
				if(pos1[i] == '\n')
					pos1[i] = fgetc(file2);
				i++;
			}
			pos1[i] = '\0';
			i =0;
			while(i<5)
			{
				pos2[i] = fgetc(file2);
				if(pos2[i] == '\n')
					pos2[i] = fgetc(file2);
				i++;
			}
			pos2[i] = '\0';
			i = 0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[i] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			out = flip(out,pos1);
			out = flip(out,pos2);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"100"))
		{
			char * pos = new char [5];
			char * ind = new char [2];
			int i =0;
			while(i<5)
			{
				pos[i] = fgetc(file2);
				if(pos[i] == '\n')
					pos[i] = fgetc(file2);
				i++;
			}
			pos[i] = '\0';
			i = 0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[i] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			out = flip(out,pos);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"101"))
		{
			char * pos = new char [5];
			char * ind = new char [2];
			int i =0;
			while(i<5)
			{
				pos[i] = fgetc(file2);
				if(pos[i] == '\n')
					pos[i] = fgetc(file2);
				i++;
			}
			pos[i] = '\0';
			i = 0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[i] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			out = flip(out,pos);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"110"))
		{
			char * pos = new char [5];
			char * ind = new char [2];
			char * out = new char [32];
			int i =0;
			while(i<5)
			{
				pos[i] = fgetc(file2);
				if(pos[i] == '\n')
					pos[i] = fgetc(file2);
				i++;
			}
			pos[i] = '\0';
			i = 0;
			while(i<2)
			{
				ind[i] = fgetc(file2);
				if(ind[i] == '\n')
					ind[i] = fgetc(file2);
				i++;
			}
			ind[i] = '\0';
			if (!strcmp(ind,d1->index))
				strcpy(out,d1->val);
			else if (!strcmp(ind,d2->index))
				strcpy(out,d2->val);
			else if (!strcmp(ind,d3->index))
				strcpy(out,d3->val);
			else if (!strcmp(ind,d4->index))
				strcpy(out,d4->val);
			out = flip(out,pos);
			cout<<out<<"\n";
		}
		else if(!strcmp(index,"111"))
		{
			int rep=0,i=0;
			char * repchar = new char [3];
			while(i<3)
			{
				repchar[i] = fgetc(file2);
				if(repchar[i] == '\n')
					repchar[i] = fgetc(file2);
				i++;
			}
			rep = btod3(repchar);
			i=0;
			while(i<rep)
			{
				cout<<out<<"\n";
				i++;
			}
		}
		else if(!strcmp(index,"222"))
			break;
		

	}while(1);

	//cout<<"\nTEST\n";
	//cout<<flip("11111111111111111111111111111111","00010");
	cin>>a;
	return 0;
}


char * getaline(FILE * fp)
{
	char * reply = new char[40];
	int i = 0;
	reply[i] = fgetc(fp);
	if(reply[i] == EOF)
		return NULL;
	while(reply[i] != '\n' && reply[i] != '\0' && reply[i] != EOF)
	{
		i++;
		reply[i] = fgetc(fp);
	}
	reply[i] = '\0';
	return reply;
}

char * compress(char * inp, dictionary *dic)
{
	char * t = new char[36];
	int e=5;
	int min = 32;
	int a = xorcount(inp,dic->entry[0]);
	int b = xorcount(inp,dic->entry[1]);
	int c = xorcount(inp,dic->entry[2]);
	int d = xorcount(inp,dic->entry[3]);
	if(min > a)
	{	min = a;
		e = 0;
	}
	if(min > b)
	{
		min = b;
		e = 1;
	}
	if(min > c)
	{
		min = c;
		e = 2;
	}
	if(min > d)
	{	min = d;
		e = 3;
	}
	//cout<<"MIN = "<<min;
	if(min < 5) 
	{
		if (min == 0)
		{
			strcpy(t,"001");
		//	cout<<"0th - ";

			switch(e)
			{
				case 0: strcat(t,"00");
						break;
				case 1: strcat(t,"01");
						break;
				case 2: strcat(t,"10");
						break;
				case 3: strcat(t,"11");			
						break;
			}
			t[5]='\0';
			return t;
		}

		else if(min == 1)
		{
			int loc = xorloc(inp,dic->entry[e],0);
			strcpy(t,"010");
			strcat(t,bin(loc));
		//	cout<<"1st - ";

			switch(e)
			{
				case 0: strcat(t,"00");
						break;
				case 1: strcat(t,"01");
						break;
				case 2: strcat(t,"10");
						break;
				case 3: strcat(t,"11");			
						break;
			}
			t[10]='\0';
			return t;
		}
		else if(min == 2)
		{
			int loc1 = xorloc(inp,dic->entry[e],0);
			int loc2 = xorloc(inp,dic->entry[e],loc1+1);
		//	cout<<"2nd - ";
			if((loc2 - loc1) != 1)
			{
				strcpy(t,"011");
				strcat(t,bin(loc1));
				strcat(t,bin(loc2));
				switch(e)
				{
					case 0: strcat(t,"00");
							break;
					case 1: strcat(t,"01");
							break;
					case 2: strcat(t,"10");
							break;
					case 3: strcat(t,"11");			
							break;
				}
				t[15]='\0';
				return t;
			}
			else
			{
				strcpy(t,"100");
				strcat(t,bin(loc1));
				switch(e)
				{
					case 0: strcat(t,"00");
							break;
					case 1: strcat(t,"01");
							break;
					case 2: strcat(t,"10");
							break;
					case 3: strcat(t,"11");			
							break;
				}
				t[10]='\0';
				return t;
			}

		}
		else if(min == 3)
		{
			int loc1 = xorloc(inp,dic->entry[e],0);
			int loc2 = xorloc(inp,dic->entry[e],loc1+1);
			int loc3 = xorloc(inp,dic->entry[e],loc2+1);
		//	cout<<"3rd - ";
			if(loc3 - loc1 ==2)
			{
				strcpy(t,"101");
				strcat(t,bin(loc1));
				switch(e)
				{
					case 0: strcat(t,"00");
							break;
					case 1: strcat(t,"01");
							break;
					case 2: strcat(t,"10");
							break;
					case 3: strcat(t,"11");			
							break;
				}
				t[10]='\0';
				return t;
			}
		}
		else if(min == 4)
		{
			int loc1 = xorloc(inp,dic->entry[e],0);
			int loc2 = xorloc(inp,dic->entry[e],loc1+1);
			int loc3 = xorloc(inp,dic->entry[e],loc2+1);
			int loc4 = xorloc(inp,dic->entry[e],loc3+1);
			
		//	cout<<"4th - ";
			if(loc4 - loc1 == 3)
			{
				strcpy(t,"110");
				strcat(t,bin(loc1));
				switch(e)
				{
					case 0: strcat(t,"00");
							break;
					case 1: strcat(t,"01");
							break;
					case 2: strcat(t,"10");
							break;
					case 3: strcat(t,"11");			
							break;
				}
				t[10]='\0';
				return t;
			}
		}

	}
	strcpy(t,"000");
	strcat(t,inp);
	strcat(t,"\0");
	//cout<<"temp = "<<t;
	return t;
}

char * rlecompress(char *inp,dictionary *dic,int ctr)
{
	char * t = new char[6];
	strcpy(t,"111");
	strcat(t,bin3(ctr-1));
	t[6] = '\0';
	return t;
}

int xorcount(char *inp, dictionarynode * d)
{
	int count=0;
	for(int i = 0 ; i<32 ; i++)
	{
		if(inp[i] != d->val[i])
		{
			count ++;
		}

	}
	return count;
}

int xorloc(char *inp,dictionarynode *d,int i)
{
	for(;i<32;i++)
	{
		if(inp[i] != d->val[i])
			return i;
	}

}

char * bin(int i)
{
	
            char * result = new char[5];
			char * intermidiate = new char[5];
			char * finalres = new char[5];
			strcpy(result,"");
			strcpy(intermidiate,"");
            strcpy(finalres,"");
            int k, count=0;
            while (i != 0) 
            {
                k = i % 2;
                if(k == 0)
					strcat(result,"0");
				else
					strcat(result,"1");
				i = i / 2;
				count++;
            }
		//	cout<<result<<"\n";
			for(int x = 0, y = count-1; x<=(count/2);x++,y--)
			{	intermidiate[y] = result[x];
				intermidiate[x] = result[y];
			}
			for(int i = 5 - strlen(result); i>0 ; i--)
				strcat(finalres,"0");
			
			strcat(finalres,intermidiate);
			finalres[5] = '\0';
            return finalres;
        
}

char * bin3(int i)
{
	
            char * result = new char[3];
			char * intermidiate = new char[3];
			char * finalres = new char[3];
			strcpy(result,"");
			strcpy(intermidiate,"");
            strcpy(finalres,"");
            int k, count=0;
            while (i != 0) 
            {
                k = i % 2;
                if(k == 0)
					strcat(result,"0");
				else
					strcat(result,"1");
				i = i / 2;
				count++;
            }
		//	cout<<result<<"\n";
			for(int x = 0, y = count-1; x<=(count/2);x++,y--)
			{	intermidiate[y] = result[x];
				intermidiate[x] = result[y];
			}
			for(int i = 3 - strlen(result); i>0 ; i--)
				strcat(finalres,"0");
			
			strcat(finalres,intermidiate);
			finalres[3] = '\0';
            return finalres;
        
}

char * flip(char *in, char * pos)
{
	char * ret = new char[32];
	int loc = btod5(pos);
	//cout<<"val "<<in<<" loc "<<loc;
	strcpy(ret,in);
	if(ret[loc] == '0')
		ret[loc] = '1';
	else
		ret[loc] = '0';
	//cout<<" val "<<ret;
	return ret;
}

char * flip2(char *in, char * pos)
{
	char * ret = new char[32];
	int loc = btod5(pos);
	strcpy(ret,in);
	if(ret[loc] == '0')
		ret[loc] = '1';
	else
		ret[loc] = '0';
	if(ret[loc+1] == '0')
		ret[loc+1] = '1';
	else
		ret[loc+1] = '0';
	return ret;
}
char * flip3(char *in, char * pos)
{
	char * ret = new char[32];
	int loc = btod5(pos);
	strcpy(ret,in);
	if(ret[loc] == '0')
		ret[loc] = '1';
	else
		ret[loc] = '0';
	if(ret[loc+1] == '0')
		ret[loc+1] = '1';
	else
		ret[loc+1] = '0';
	if(ret[loc+2] == '0')
		ret[loc+2] = '1';
	else
		ret[loc+2] = '0';
	return ret;
}
char * flip4(char *in, char * pos)
{
	char * ret = new char[32];
	int loc = btod5(pos);
	strcpy(ret,in);
	if(ret[loc] == '0')
		ret[loc] = '1';
	else
		ret[loc] = '0';
	if(ret[loc+1] == '0')
		ret[loc+1] = '1';
	else
		ret[loc+1] = '0';
	if(ret[loc+2] == '0')
		ret[loc+2] = '1';
	else
		ret[loc+2] = '0';
	if(ret[loc+3] == '0')
		ret[loc+3] = '1';
	else
		ret[loc+3] = '0';
	return ret;
}


int btod5(char * p)
{
	int res=0;
	for(int i=0,j=4;i<5;i++,j--)
	{
	   if(p[i]== '1')
		   res += pow(2,j);
	        
	}
	return res;
}

int btod3(char * p)
{
	int res=0;
	for(int i=0,j=2;i<3;i++,j--)
	{
	   if(p[i]== '1')
		   res += pow(2,j);
	        
	}
	return res;
}