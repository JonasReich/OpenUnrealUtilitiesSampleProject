// Copyright (c) 2021 Jonas Reich

using UnrealBuildTool;
using System.Collections.Generic;

public class SampleProjectTarget : TargetRules
{
	public SampleProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SampleProject" } );
	}
}
