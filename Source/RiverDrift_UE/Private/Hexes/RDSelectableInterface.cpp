// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexes/RDSelectableInterface.h"

// Add default functionality here for any ISelectable functions that are not pure virtual.

void IRDSelectableInterface::SelectObject()
{
	UE_LOG(LogTemp, Log, TEXT("select object virtual version called. this should be overridden (?)"))
}

void IRDSelectableInterface::DeselectObject()
{
	UE_LOG(LogTemp, Log, TEXT("deselect object \"virtual\" version called. this should be overridden(? )"))
}
