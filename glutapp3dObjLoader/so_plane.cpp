
# include "so_plane.h"

SoPlane::SoPlane()
{
	_numpoints = 0;
	_flatn = false;
}

void SoPlane::init(std::string _path)
{
	// Load programs:
	//_vshgou.load_and_compile(GL_VERTEX_SHADER, "../mcol_phong.vert");
	//_fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../mcol_phong.frag");
	_vshtex.load_and_compile(GL_VERTEX_SHADER, "../texgouraud.vert");
	_fshtex.load_and_compile(GL_FRAGMENT_SHADER, "../texgouraud.frag");
	_proggouraud.init_and_link(_vshtex, _fshtex);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(3);       // will use 3 buffers

	_proggouraud.uniform_locations(9); // will send 9 variables
	_proggouraud.uniform_location(0, "vTransf");
	_proggouraud.uniform_location(1, "vProj");
	_proggouraud.uniform_location(2, "lPos");
	_proggouraud.uniform_location(3, "la");
	_proggouraud.uniform_location(4, "ld");
	_proggouraud.uniform_location(5, "ls");
	_proggouraud.uniform_location(6, "ka");
	_proggouraud.uniform_location(7, "ks");
	_proggouraud.uniform_location(8, "sh");

	GsImage I;
	if (!I.load(_path.c_str())) {
		std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1);
	}
	else
		std::cout << "LOADED IMAGE\n";

	glGenTextures(1, &_texid);
	glBindTexture(GL_TEXTURE_2D, _texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	//std::cout<< "before texid: " << _texid << std::endl;

	I.init(0, 0);
}

void SoPlane::build(float xmin, float xmax, float zmin, float zmax, float y)
{

	// make initial angles and coordinates where needed
	GsVec dir;
	int i;
	GsVec line;

	P.size(0); C.size(0); N.size(0); NL.size(0); T.size(0);
	for (int j = xmin; j < xmax; j++) {
		for (int k = zmin; k < zmax; k++) {
			P.push(GsVec(j+1, y, k+1)); P.push(GsVec(j, y, k+1)); P.push(GsVec(j, y, k));
			T.push(GsVec2((j+1-xmin)/(xmax-xmin), (k+1-zmin)/(zmax-zmin))); T.push(GsVec2((j-xmin)/(xmax-xmin),(k+1-zmin)/(zmax-zmin))); T.push(GsVec2((j-xmin)/(xmax-xmin), (k-zmin)/(zmax-zmin)));
			dir = cross((GsVec(j, y, k) - GsVec(j+1, y, k+1)), (GsVec(j, y, k+1) - GsVec(j+1, y, k+1)));
			N.push(dir / dir.len()); N.push(dir / dir.len()); N.push(dir / dir.len());
			line = (GsVec(j+1, y, k+1) + GsVec(j, y, k+1) + GsVec(j, y, k)) / 3.0f;
			NL.push(line); NL.push(line + dir / dir.len());

			//triangle 2 of face
			P.push(GsVec(j+1, y, k+1)); P.push(GsVec(j, y, k)); P.push(GsVec(j+1, y, k));
			T.push(GsVec2((j+1-xmin)/(xmax-xmin), (k+1-zmin)/(zmax-zmin))); T.push(GsVec2((j-xmin)/(xmax-xmin), (k-zmin)/(zmax-zmin))); T.push(GsVec2((j+1-xmin)/(xmax-xmin), (k-zmin)/(zmax-zmin)));
			dir = cross((GsVec(j+1, y, k) - GsVec(j+1, y, k+1)), (GsVec(j, y, k) - GsVec(j+1, y, k+1)));
			N.push(dir / dir.len()); N.push(dir / dir.len()); N.push(dir / dir.len());
			line = (GsVec(j+1, y, k+1) + GsVec(j, y, k) + GsVec(j+1, y, k)) / 3.0f;
			NL.push(line); NL.push(line + dir / dir.len());
		}
	}
	C.size(P.size()); C.setall(GsColor::darkgreen);
	_mtl.specular.set(255, 255, 255);
	_mtl.shininess = 8; // increase specular effect

	// send data to OpenGL buffers:
	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float)*T.size(), T.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0); // break the existing vertex array object binding.

	// save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0); N.capacity(0);  T.capacity(0);
	// (leave NL untouched because it will be used by another class for display)
}

void SoPlane::draw(const GsMat& tr, const GsMat& pr, const GsLight& l)
{
	float f[4];
	float sh = (float)_mtl.shininess;
	if (sh<0.001f) sh = 64;

	glUseProgram(_proggouraud.id);
	glBindVertexArray(va[0]);
	glBindTexture(GL_TEXTURE_2D, _texid);
	glUniformMatrix4fv(_proggouraud.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_proggouraud.uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(_proggouraud.uniloc[2], 1, l.pos.e);
	glUniform4fv(_proggouraud.uniloc[3], 1, l.amb.get(f));
	glUniform4fv(_proggouraud.uniloc[4], 1, l.dif.get(f));
	glUniform4fv(_proggouraud.uniloc[5], 1, l.spe.get(f));
	glUniform4fv(_proggouraud.uniloc[6], 1, _mtl.ambient.get(f));
	//glUniform4fv(_proggouraud.uniloc[7], 1, _mtl.diffuse.get(f));
	glUniform4fv(_proggouraud.uniloc[7], 1, _mtl.specular.get(f));
	glUniform1fv(_proggouraud.uniloc[8], 1, &sh);


	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
}

