rollout ExportSkinDialog "Export Skin" width:300
(
	button ExportSkinButton "Export Skin"
	
	on ExportSkinButton pressed do
	(
		objMesh = undefined
		objSkin = undefined
		
		max modify mode
		
		for obj in objects do
		(
			if (canConvertTo obj TriMeshGeometry) then
			(
				if objMesh == undefined then
				(
					for i = 1 to obj.modifiers.count do
					(
						if obj.modifiers [i].Name == "Skin" then
						(
							objMesh = obj
							objSkin = obj.modifiers [i]
						)
					)
				)
			)
		)
		
		if (objMesh != undefined) and (objSkin != undefined) then
		(
			FileName = GetSaveFileName types:"dxframe skin (*.dxfskin)|*.dxfskin|"
			
			if (FileName != undefined) then
			(
				file = fopen FileName "wb"
				
				WriteString file (objMesh.Name)
				
				fseek file 256 #seek_set
				
				WriteLong file (objMesh.numVerts)
				WriteLong file (objMesh.numFaces)
				WriteLong file (objMesh.numTVerts)
				
				start = animationRange.start.frame as integer
				
				at time start
				(
					for i = 1 to objMesh.numVerts do
					(
						p = (GetVert objMesh i)
						WriteFloat file p.x
						WriteFloat file p.y
						WriteFloat file p.z
					)
				)
				
				for i = 1 to objMesh.numFaces do
				(
					f = (GetFace objMesh i) - [1, 1, 1]
					WriteLong file (f.x as Integer)
					WriteLong file (f.y as Integer)
					WriteLong file (f.z as Integer)
				)
				
				fclose file
				
				messageBox ("Skin was exported.") title:"Message"
			)
		)
	)
)
				  
CreateDialog ExportSkinDialog