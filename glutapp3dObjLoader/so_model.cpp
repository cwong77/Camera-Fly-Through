
# include "so_model.h"

SoModel::SoModel()
 {
   _numpoints = 0;
   _phong = false;
 }

void SoModel::init (std::string _path)
 {
   _vshtex.load_and_compile ( GL_VERTEX_SHADER, "../texgouraud.vert" );
   _fshtex.load_and_compile ( GL_FRAGMENT_SHADER, "../texgouraud.frag" );
   _progtex.init_and_link (_vshtex, _fshtex);

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 3 );       // will use 3 buffers

   _progtex.uniform_locations ( 9 ); // will send 9 variables
   _progtex.uniform_location ( 0, "vTransf" );
   _progtex.uniform_location ( 1, "vProj" );
   _progtex.uniform_location ( 2, "lPos" );
   _progtex.uniform_location ( 3, "la" );
   _progtex.uniform_location ( 4, "ld" );
   _progtex.uniform_location ( 5, "ls" );
   _progtex.uniform_location ( 6, "ka" );
   _progtex.uniform_location ( 7, "ks" );
   _progtex.uniform_location ( 8, "sh" );

   //first image
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

void SoModel::build ( GsModel& m )
 {

	std::cout << "Build model\n";

   int i;
   GsColor c;
   P.size(0); T.size(0); N.size(0); NL.size(0);

   /* There are multiple ways to organize data to send to OpenGL. 
      Here we send material information per vertex but we only send the diffuse color
      information per vertex. The other components are uniforms sent to affect the whole object.
      This is a solution that keeps this code simple and is ok for most objects.
   */

   // build arrays:
   for ( i=0; i<m.F.size(); i++ )
    { GsModel::Face& f = m.F[i];
      P.push()=m.V[f.a]; P.push()=m.V[f.b]; P.push()=m.V[f.c]; 

      if ( m.Fn.size()>0 && i<m.Fn.size() )
       { GsModel::Face& f = m.Fn[i];
         N.push()=m.N[f.a]; N.push()=m.N[f.b]; N.push()=m.N[f.c];
	  }
      else if ( m.N.size()>0 && i<m.N.size() )
       { GsVec n=m.N[i]; N.push()=n; N.push()=n; N.push()=n; }
      else
       { GsVec n=m.face_normal(i); N.push()=n; N.push()=n; N.push()=n; }

	  if (m.Ft.size() > 0 && i < m.Ft.size()) {
		  GsModel::Face& f = m.Ft[i];
		  T.push() = m.T[f.a]; T.push() = m.T[f.b]; T.push() = m.T[f.c];
	  }
	  /*
      c = GsColor::gray;
      if ( m.Fm.size()>0 && i<m.Fm.size() ) 
       { int id=m.Fm[i]; 
         if (id<0) { if (C.size()>0)c=C.top(); } else c=m.M[id].diffuse;
         //std::cout<<i<<"/"<<id<<" "<<c<<"\n";
       }
      else if ( m.M.size()>0 && i<m.M.size() ) 
       { c = m.M[i].diffuse; }
      C.push()=c; C.push()=c; C.push()=c;
	  */
    }
   //for (int i = 0; i < N.size(); ++i) {
	//   std::cout << N[i] << std::endl;
//   }
   //std::cout << "is there stuff in my texure coords?\n";
   //for (int i = 0; i < T.size(); ++i) {
	 //  std::cout << T[i] << std::endl;
   //}
   //for (int i = 0; i < m.Ft.size(); ++i) {
	 //  std::cout << m.Ft[i] << std::endl;
   //}

   for (int i = 0; i < P.size(); ++i) {
	   NL.push(P[i]); NL.push(P[i] + N[i]*.01);
   }
   
   std::cout << "NLsize: " << NL.size() << std::endl;
   std::cout << "Tsize: " << T.size() << std::endl;
   std::cout << "Psize: " << P.size() << std::endl;
   std::cout << "Nsize: " << N.size() << std::endl;

   //std::cout << "m.M.size(): " << m.M.size() << std::endl;
   if (m.M.size() > 0) { _mtl = m.M[0]; }
   else _mtl.init();

   // send data to OpenGL buffers:
   glBindVertexArray ( va[0] );
   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );
   glEnableVertexAttribArray ( 2 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[2] );
   glBufferData ( GL_ARRAY_BUFFER, 2*sizeof(float)*T.size(), T.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindVertexArray(0); // break the existing vertex array object binding.

   std::cout<<"build ok.\n";

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.capacity(0); T.capacity(0); N.capacity(0);
 }

void SoModel::draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
 {
	float f[4];
	float sh = (float)_mtl.shininess;
	if (sh<0.001f) sh = 64;

	glUseProgram(_progtex.id);
	glBindVertexArray(va[0]);
	glBindTexture(GL_TEXTURE_2D, _texid);
	glUniformMatrix4fv(_progtex.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_progtex.uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(_progtex.uniloc[2], 1, l.pos.e);
	glUniform4fv(_progtex.uniloc[3], 1, l.amb.get(f));
	glUniform4fv(_progtex.uniloc[4], 1, l.dif.get(f));
	glUniform4fv(_progtex.uniloc[5], 1, l.spe.get(f));
	glUniform4fv(_progtex.uniloc[6], 1, _mtl.ambient.get(f));
	glUniform4fv(_progtex.uniloc[7], 1, _mtl.diffuse.get(f));
	glUniform4fv(_progtex.uniloc[8], 1, _mtl.specular.get(f));
	glUniform1fv(_progtex.uniloc[9], 1, &sh);

    glBindVertexArray ( va[0] );
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
 }

