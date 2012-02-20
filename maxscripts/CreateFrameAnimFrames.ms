rollout CreateObjDialog "Create Frames For Frame Anim" width:300
(
	editText AnimName "Anim Name: "
	
	spinner SpinnerNumFrames "Number of frames: " type:#integer width:80 align:#right range:[1, 10000, 30]
	
	spinner SpinnerWidth "Width: " width:80 align:#right range:[0, 1000000, 100]
	spinner SpinnerLength "Length: " width:80 align:#right range:[0, 1000000, 100]
	
	button CreateFramesButton "Create Frames"
	
	on CreateFramesButton pressed do
	(
		for i = 0 to SpinnerNumFrames.value do
		(
			plane width:SpinnerWidth.value length:SpinnerLength.value pos:[0, 0, i * 10] name:(AnimName.text + "_" + i as string)
		)
		
		ids = $.material.materialIDList --Go through the material ids
		for id in ids do
		(
			$.selectbymaterial id --Select by material id
			_faceSel = polyOp.getFaceSelection $ -- Get the selection
			polyOp.detachFaces $ _faceSel delete:true asNode:true -- Detach as separate object
		)
	)
)

createDialog  CreateObjDialog