/**
 * @author ARTSAT     / https://github.com/ARTSAT
 * @author motokimura / https://github.com/motokimura
 *
 * Description: A library to handle ARTSAT1:INVADER as the 3D object in Three.js
 *
 * Limitations:
 *
 * Usage:
 *  var scene = new THREE.Scene ();
 *  var invader = new Invader (scene);
 */

// Constructer
var Invader = function (scene) {
	
	this.meshFrame;			// mesh for the frame
	this.meshAntennas;		// mesh for the antennas (tx/rx)
	this.meshPannel = [];	// meshes for the solar pannels on each face
	
	this.loadAssemblies (scene);
	
};

// Constants
Invader.MinusX = 0;
Invader.MinusY = 1;
Invader.MinusZ = 2;
Invader.PlusX  = 3;
Invader.PlusY  = 4;
Invader.PlusZ  = 5;

// Methods
Invader.prototype = {
	
	// function to check if loading is finished
	isLoaded : function () {
		
		if (!this.meshFrame.visible) {
			return false;
		}
		if (!this.meshAntennas.visible) {
			return false;
		}
		for (var i = 0; i < 6; i++) {
			if (!this.meshPannel[i].visible) {
				return false;
			}
		}
		
		return true;
	},
	
	
	// function to resize INVADER
	resize : function (x, y, z) {
		
		if (!this.isLoaded) {
			return;
		}
		
		this.meshFrame.scale.set (x, y, z);
		this.meshAntennas.scale.set (x, y, z);
		
		for (var i = 0; i < 6; i++) {
			this.meshPannel[i].scale.set (x, y, z);
		}
	},
	
	
	// function to rotate INVADER
	rotate : function (dx, dy, dz) {
		
		if (!this.isLoaded) {
			return;
		}
		
		this.rotateX (dx);
		this.rotateY (dy);
		this.rotateZ (dz);
	},
	
	// function to rotate INVADER along x-axis
	rotateX : function (dtheta) {
		
		if (!this.isLoaded) {
			return;
		}
		
		this.meshFrame.rotation.x += dtheta;
		this.meshAntennas.rotation.x += dtheta;
		
		for (var i = 0; i < 6; i++) {
			this.meshPannel[i].rotation.x += dtheta;
		}
	},
	
	// function to rotate INVADER along y-axis
	rotateY : function (dtheta) {
		
		if (!this.isLoaded) {
			return;
		}
		
		this.meshFrame.rotation.y += dtheta;
		this.meshAntennas.rotation.y += dtheta;
		
		for (var i = 0; i < 6; i++) {
			this.meshPannel[i].rotation.y += dtheta;
		}
	},
	
	// function to rotate INVADER along z-axis
	rotateZ : function (dtheta) {
		
		if (!this.isLoaded) {
			return;
		}
		
		this.meshFrame.rotation.z += dtheta;
		this.meshAntennas.rotation.z += dtheta;
		
		for (var i = 0; i < 6; i++) {
			this.meshPannel[i].rotation.z += dtheta;
		}
	},
	
	
	// function to load .stl files
	loadAssemblies : function (scene) {
		
		this.loadFrame (scene);
		this.loadAntennas (scene);
		this.loadPannels (scene);
	},
	
	// function to load .stl file of the frame
	loadFrame : function (scene) {
		
		var scope = this;
	
		var loader = new THREE.STLLoader ();
		loader.addEventListener ('load', function (event) {
				
			var geometry = event.content;
			var material = new THREE.MeshPhongMaterial ({ambient: 0xffffff, color: 0xffffff, specular: 0xffffff, shininess: 50});
			scope.meshFrame = new THREE.Mesh (geometry, material);
			
			scope.meshFrame.castShadow = true;
			scope.meshFrame.receiveShadow = true;
			
			scene.add (scope.meshFrame);
		});
		loader.load ('../models/invader_frame.stl');
	},
	
	// function to load .stl file of antennas
	loadAntennas : function (scene) {
		
		var scope = this;
	
		var loader = new THREE.STLLoader ();
		loader.addEventListener ('load', function (event) {
				
			var geometry = event.content;
			var material = new THREE.MeshPhongMaterial ({ambient: 0x555555, color: 0x3A322B, specular: 0x95411C, shininess: 200});
			scope.meshAntennas = new THREE.Mesh (geometry, material);
			
			scope.meshAntennas.castShadow = true;
			scope.meshAntennas.receiveShadow = true;
			
			scene.add (scope.meshAntennas);
		});
		loader.load ('../models/invader_antennas.stl');
	},
	
	// function to load .stl file of solar pannels
	loadPannels : function (scene) {
		
		var scope = this;
		var p_num = 0;
	
		var loader = new THREE.STLLoader ();
		loader.addEventListener ('load', function (event) {
				
			var geometry = event.content;
			var material = new THREE.MeshPhongMaterial ({ambient: 0x000000, color: 0x000000, specular: 0x171780, shininess: 100});
			
			scope.meshPannel.push (new THREE.Mesh (geometry, material));
			
			scope.meshPannel[p_num].castShadow = true;
			scope.meshPannel[p_num].receiveShadow = true;
			
			scene.add (scope.meshPannel[p_num]);
			
			p_num++;
		});
		for (var i = 0; i < 6; i++) {
			
			if (i == Invader.MinusX) {
				loader.load ('../models/invader_pannel_mx.stl');
			}
			else if (i == Invader.MinusY) {
				loader.load ('../models/invader_pannel_my.stl');
			}
			else if (i == Invader.MinusZ) {
				loader.load ('../models/invader_pannel_mz.stl');
			}
			else if (i == Invader.PlusX) {
				loader.load ('../models/invader_pannel_px.stl');
			}
			else if (i == Invader.PlusY) {
				loader.load ('../models/invader_pannel_py.stl');
			}
			else if (i == Invader.PlusZ) {
				loader.load ('../models/invader_pannel_pz.stl');
			}
		}
	}
};


