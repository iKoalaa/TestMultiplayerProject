// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FDelegatesHolder final
{
	
public:
	static FDelegatesHolder& Get();
	static void Destroy();

private:
	FDelegatesHolder() = default;
	FDelegatesHolder(const FDelegatesHolder&) = delete;
	FDelegatesHolder(FDelegatesHolder&&) = delete;

	FDelegatesHolder& operator=(const FDelegatesHolder&) = delete;
	FDelegatesHolder& operator=(FDelegatesHolder&&) = delete;

	~FDelegatesHolder() =default;
	
	static class FDelegatesHolder* Instance;
};

