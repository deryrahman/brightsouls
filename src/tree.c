/*
Author
Nama 	: Dery Rahman A
NIM		: 13515097
Tanggal	: 07/11/2016
File	: tree.c
*/

#include <stdio.h>
#include "tree.h" // masih dijalankan terpisah dari main utama

Tree TreeCreate(){
	return TreeAlloc(SkillCreate(0,0,0));
}
TreeAddress TreeAlloc(Skill X){
	TreeAddress P=(TreeAddress) malloc(sizeof(Node));
	if(P!=Nil){
		TreeRoot(P)=X;
		TreeLeft(P)=Nil;
		TreeRight(P)=Nil;
	}
	return P;
}
void TreeDealloc(TreeAddress P){
	free(P);
}
boolean TreeIsEmpty(Tree P){
	return P==Nil;
}
boolean TreeIsOneElmt(Tree P){
	if (!(TreeIsEmpty(P))){
		return (TreeLeft(P)==Nil) && (TreeRight(P)==Nil);
	} else {
		return false;
	}
}
void TreeAddLeft(Tree P, Skill X, TreeAddress *Pl){
	TreeAddress PLeft=TreeAlloc(X);
	TreeLeft(P)=PLeft;
	*Pl=PLeft;
}
void TreeAddRight(Tree P, Skill X, TreeAddress *Pr){
	TreeAddress PRight=TreeAlloc(X);
	TreeRight(P)=PRight;
	*Pr=PRight;
}
void TreePrint(Tree P, int curr){
	if(P==Nil){}
	if ((TreeLeft(P) == Nil) || (TreeRight(P) == Nil)) {
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|-------------------- Min EXP : %u\n", TreeRoot(P).exp_req);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Att : ++%u\n",TreeRoot(P).attack);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Def : ++%u\n",TreeRoot(P).deffense);
	} else {
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|-------------------- Min EXP : %u\n", TreeRoot(P).exp_req);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Att : ++%u\n",TreeRoot(P).attack);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Def : ++%u\n",TreeRoot(P).deffense);
		TreePrint(TreeLeft(P),curr+1);
		TreePrint(TreeRight(P),curr+1);
	}
}
void TreePrintActivated(Tree P, int curr){
	if (P == Nil)
		return;
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))) {
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|-------------------- Min EXP : %u\n", TreeRoot(P).exp_req);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Att : ++%u\n",TreeRoot(P).attack);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Def : ++%u\n",TreeRoot(P).deffense);
		} else {
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|-------------------- Min EXP : %u\n", TreeRoot(P).exp_req);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Att : ++%u\n",TreeRoot(P).attack);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Def : ++%u\n",TreeRoot(P).deffense);
			TreePrintActivated(TreeLeft(P),curr+1);
			TreePrintActivated(TreeRight(P),curr+1);
		}
	}
}

uint SkillTotalAttack(Tree P){
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))){
			return TreeRoot(P).attack;
		} else {
			return TreeRoot(P).attack+SkillTotalAttack(TreeLeft(P))+SkillTotalAttack(TreeRight(P));
		}
	}
	return 0;
}

uint SkillTotalDeffense(Tree P){
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))){
			return TreeRoot(P).deffense;
		} else {
			return TreeRoot(P).deffense+SkillTotalDeffense(TreeLeft(P))+SkillTotalDeffense(TreeRight(P));
		}
	}
	return 0;
}

void SkillActivatedGenerate(Tree *P, int EXP){
	if (*P == Nil){}
	else if (EXP<TreeRoot(*P).exp_req) {
	} else {
		TreeStatus(*P)=true;
		SkillActivatedGenerate(&TreeLeft(*P),EXP);
		SkillActivatedGenerate(&TreeRight(*P),EXP);
	}
}
