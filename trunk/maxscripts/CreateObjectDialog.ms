rollout CreateObjDialog "Create Object Dialog"
(
	editText AnimName "Anim Name"
	spinner SomeSpinner "XYZ:"
	button CreateObjectButton "Create Object"
	
	on CreateObjectButton pressed do
	(
		for i = 1 to 100 do
		(
			plane width:SomeSpinner.value length:SomeSpinner.value pos: [i * 100, 0, 0]
		)
	)
)

createDialog  CreateObjDialog