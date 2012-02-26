rollout ExportSkinDialog "Export Skin Like DXF" width:300
(
	button ExportSkinButton "Export Skin Like DXF"
	
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
			numVerts = objMesh.numVerts;				--//кол-во вершин
			numTVerts = objMesh.numTVerts;			--//кол-во текстурных вершин
			numFaces = objMesh.numFaces;			--//кол-во полигонов (треугольников (фейсов (граней))) == кол-во текстурных полигонов
			
			if numTVerts == 0 then
			(
				messageBox ("There is no texture co-ordinates. \n Try to generate texture co-ordinates. \n (Use UVW Map modifier for example.)") title:"Error"
			)
			
			pVerts = #()			--//массив с вершинами
			pNormals = #()		--//массив с нормалями
			pTVerts = #()			--//массив с текстурными вершинами
			pFaces = #()			--//массив с индексами
			pTFaces = #()		--//массив с текстурными индексами
			
			pVerts.count = numVerts * 3
			pNormals.count = numVerts * 3
			pTVerts.count = numTVerts * 2
			pFaces.count = numFaces * 3
			pTFaces.count = numFaces * 3
			
			start = animationRange.start.frame as integer
			
			--buildNormals objMesh
			
			at time start
			(
				for i = 1 to numVerts do
				(
					v = (GetVert objMesh i)

					pVerts [1 + (i - 1) * 3] = v.x
					pVerts [2 + (i - 1) * 3] = v.y
					pVerts [3 + (i - 1) * 3] = v.z
				)
			)
			
			for i = 1 to numVerts do
			(
				v = (GetNormal objMesh i)

				pNormals [1 + (i - 1) * 3]	= v.x
				pNormals [2 + (i - 1) * 3] = v.y
				pNormals [3 + (i - 1) * 3] = v.z
			)
			
			for i = 1 to numFaces do
			(
				v = (GetFace objMesh i)

				pFaces [1 + (i - 1) * 3] = v.x
				pFaces [2 + (i - 1) * 3] = v.y
				pFaces [3 + (i - 1) * 3] = v.z
			)
			
			for i = 1 to numFaces do
			(
				v = (GetTVFace objMesh i)

				pTFaces [1 + (i - 1) * 3] = v.x
				pTFaces [2 + (i - 1) * 3] = v.y
				pTFaces [3 + (i - 1) * 3] = v.z
			)
			
			pRecomputedVerts = #()
			pRecomputedNormals = #()
			
			pRecomputedVerts.count = numTVerts * 3
			pRecomputedNormals.count = numTVerts * 3
			
			for i = 1 to numFaces * 3 do
			(
				pRecomputedVerts [1 + (pTFaces [i] - 1) * 3] = pVerts [1 + (pFaces [i] - 1) * 3]
				pRecomputedVerts [2 + (pTFaces [i] - 1) * 3] = pVerts [2 + (pFaces [i] - 1) * 3]
				pRecomputedVerts [3 + (pTFaces [i] - 1) * 3] = pVerts [3 + (pFaces [i] - 1) * 3]
			)
			
			for i = 1 to numFaces * 3 do
			(
				pRecomputedNormals [1 + (pTFaces [i] - 1) * 3] = pNormals [1 + (pFaces [i] - 1) * 3]
				pRecomputedNormals [2 + (pTFaces [i] - 1) * 3] = pNormals [2 + (pFaces [i] - 1) * 3]
				pRecomputedNormals [3 + (pTFaces [i] - 1) * 3] = pNormals [3 + (pFaces [i] - 1) * 3]
			)
			
			FileName = GetSaveFileName types:"dxframe skin (*.dxf)|*.dxf|"
			
			if (FileName != undefined) then
			(
				file = fopen FileName "wb"
				
				WriteString file (objMesh.Name)
				
				fseek file 256 #seek_set
				
				WriteLong file (objMesh.numTVerts)
				WriteLong file (objMesh.numFaces)
				
				for i = 1 to numTVerts do
				(
					--//write vertexes
					WriteFloat file (pRecomputedVerts [1 + (i - 1) * 3])
					WriteFloat file (pRecomputedVerts [2 + (i - 1) * 3])
					WriteFloat file (pRecomputedVerts [3 + (i - 1) * 3])
					
					--//write normals
					WriteFloat file (pRecomputedNormals [1 + (i - 1) * 3])
					WriteFloat file (pRecomputedNormals [2 + (i - 1) * 3])
					WriteFloat file (pRecomputedNormals [3 + (i - 1) * 3])
					
					--//write texture coordinates
					WriteFloat file (0)
					WriteFloat file (0)
				)
				
				for i = 1 to numFaces do
				(
					WriteLong file (pTFaces [1 + (i - 1) * 3] - 1)	--correcting by -1
					WriteLong file (pTFaces [2 + (i - 1) * 3] - 1)
					WriteLong file (pTFaces [3 + (i - 1) * 3] - 1)
				)
				
				--texture name
				fseek file 255 #seek_cur
				WriteByte file (0)
				
				fclose file
				
				messageBox ("Skin was exported.") title:"Message"
			)
		)
	)
)

CreateDialog ExportSkinDialog