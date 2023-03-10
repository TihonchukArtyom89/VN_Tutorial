// Copyright 2017-2018 Csaba Molnar, Daniel Butum
#include "SGraphNode_DialogueBase.h"

#include "Widgets/Layout/SBox.h"
#include "DialogueGraphNode_Root.h"
#include "IDocumentation.h"
#include "GraphEditorDragDropAction.h"

#include "SDialogueGraphPin.h"

#define LOCTEXT_NAMESPACE "DialogueEditor"


/////////////////////////////////////////////////////
// SGraphNode_DialogueBase
void SGraphNode_DialogueBase::Construct(const FArguments& InArgs, UDialogueGraphNode_Base* InNode)
{
	GraphNode = Cast<UEdGraphNode>(InNode);
	DialogueGraphNode_Base = InNode;
	Settings = GetDefault<UDlgSystemSettings>();
}

void SGraphNode_DialogueBase::CreatePinWidgets()
{
	// Create only one pin, the output one, the rest is handled by FDialogueGraphConnectionDrawingPolicy
	UEdGraphPin* OutputPin = DialogueGraphNode_Base->GetOutputPin();
	UEdGraphPin* InputPin = DialogueGraphNode_Base->GetInputPin();

	// Order is important, or something like that.
	if (!IsValidPin(InputPin))
	{
		return;
	}

	//CreateStandardPinWidget(InputPin);

	if (!IsValidPin(OutputPin))
	{
		return;
	}

	CreateStandardPinWidget(OutputPin);
}

void SGraphNode_DialogueBase::CreateStandardPinWidget(UEdGraphPin* Pin)
{
	Super::CreateStandardPinWidget(Pin);
}

void SGraphNode_DialogueBase::UpdateGraphNode()
{
	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	InputPins.Empty();
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	PinsNodeBox.Reset();
	SAssignNew(PinsNodeBox, SVerticalBox);

	// This Node visibility
	SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &Self::GetNodeVisibility)));
}

void SGraphNode_DialogueBase::SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel)
{
	check(!OwnerGraphPanelPtr.IsValid());
	SetParentPanel(OwnerPanel);
	OwnerGraphPanelPtr = OwnerPanel;
	GraphNode->DEPRECATED_NodeWidget = SharedThis(this);

	// Once we have an owner, and if hide Unused pins is enabled, we need to remake our pins to drop the hidden ones
	if (OwnerGraphPanelPtr.Pin()->GetPinVisibility() != SGraphEditor::Pin_Show && PinsNodeBox.IsValid())
	{
		PinsNodeBox->ClearChildren();
		CreatePinWidgets();
	}
}

TSharedPtr<SGraphPin> SGraphNode_DialogueBase::CreatePinWidget(UEdGraphPin* Pin) const
{
	// Called by CreateStandardPinWidget
	return SNew(SDialogueGraphPin, Pin);
}

void SGraphNode_DialogueBase::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	PinsNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];

	if (PinToAdd->GetDirection() == EGPD_Output)
	{
		OutputPins.Add(PinToAdd);
	}
	else
	{
		InputPins.Add(PinToAdd);
	}
}
// End SGraphNode interface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
