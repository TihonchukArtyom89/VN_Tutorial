
// Copyright 2017-2018 Csaba Molnar, Daniel Butum
#pragma once

#include "CoreMinimal.h"

#include "FileHelpers.h"
#include "DlgDialogue.h"
#include "DlgManager.h"


class FDlgCommandletHelper
{
public:
	static bool SaveAllDirtyDialogues()
	{
		// Save all dirty packages
		constexpr bool bPromptUserToSave = false;
		constexpr bool bFastSave = true;
		constexpr bool bNotifyNoPackagesSaved = false;
		constexpr bool bCanBeDeclined = false;
		static TArray<UClass*> SaveContentClasses = { UDlgDialogue::StaticClass() };
		return FEditorFileUtils::SaveDirtyContentPackages(SaveContentClasses, bPromptUserToSave, bFastSave, bNotifyNoPackagesSaved, bCanBeDeclined);
	}

	static bool SaveAllDialogues()
	{
		TArray<UDlgDialogue*> Dialogues = UDlgManager::GetAllDialoguesFromMemory();
		TArray<UPackage*> PackagesToSave;
		for (UDlgDialogue* Dialogue : Dialogues)
		{
			Dialogue->OnAssetSaved();
			Dialogue->MarkPackageDirty();
			PackagesToSave.Add(Dialogue->GetOutermost());
		}

		static constexpr bool bCheckDirty = false;
		return UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, bCheckDirty);
	}

	static bool IsDialoguePathInProjectDirectory(const FString& DialoguePath)
	{
		return DialoguePath.StartsWith(TEXT("/Game"));
	}
};
