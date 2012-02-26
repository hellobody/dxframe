rollout ExportBonesDialog "Export Sceleton" width:300
(
	button ExportBonesButton "Export Sceleton"
	
	on ExportBonesButton pressed do
	(
		local allBones = #()
		
		for obj in objects do
		(
			if ((classOf obj) == BoneGeometry) then
			(
				append allBones obj
			)
		)
		
		if (allBones.count > 0) then
		(
			FileName = GetSaveFileName types:"dxframe sceleton (*.dxfskeleton)|*.dxfskeleton|"
			
			if (FileName != undefined) then
			(
				file = fopen FileName "wb"
				
				start = animationRange.start.frame as integer
				end = animationRange.end.frame as integer
				
				WriteShort file (allBones.count)
				WriteShort file (end - start + 1)
				
				for bn in allBones do
				(
					at time start m0 = (inverse bn.objectTransform)
					for frame = start to end do
					(
						at time frame
						(
							m = m0 * bn.objectTransform
							p = m.Position
							r = m.Rotation
							
							WriteFloat file p.x
							WriteFloat file p.y
							WriteFloat file p.z
							
							WriteFloat file r.x
							WriteFloat file r.y
							WriteFloat file r.z
							WriteFloat file r.w
						)
					)
				)
				
				fclose file
				messageBox (allBones.count as string + " bones was exported.") title:"Message"
			)
			else messageBox ("File open error.") title:"Message"
		)
		else messageBox ("No bones found in the scene.") title:"Message"
	)
)
				  
CreateDialog ExportBonesDialog