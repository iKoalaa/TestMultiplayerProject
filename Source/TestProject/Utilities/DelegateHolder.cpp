// Fill out your copyright notice in the Description page of Project Settings.


#include "DelegateHolder.h"

FDelegatesHolder* FDelegatesHolder::Instance = nullptr;

FDelegatesHolder& FDelegatesHolder::Get()
{
	if (nullptr == Instance)
	{
		Instance = new FDelegatesHolder();
		FCoreDelegates::OnExit.AddStatic(Destroy);
	}

	return *Instance;
}

void FDelegatesHolder::Destroy()
{
	delete Instance;
	Instance = nullptr;
}


