//
//  StarButton.h
//
//  Created by Sungwoo Choi on 7/30/15.
//  Copyright (c) 2015 SungLab. All rights reserved.
//


#ifndef STARBUTTON_H
#define STARBUTTON_H


#include <stdio.h>
#include "../StarMain.h"

//Typedef
typedef enum 
{
	INTERFACE_NONE,
	INTERFACE_CIRCLE,
	INTERFACE_RECT_VERTICAL,
	INTERFACE_RECT_HORIZONTAL
}STAR_INTERFACE;

class StarButtonDelegate
{
public:
    virtual void Callback_Press(int idx){};
};

class StarButton:public StarView
{

public:

	/*
	* Button Properties
	*/
	StarButtonDelegate* delegate;

	std::vector<Vec3> button_position;
	std::vector<Vec3> button_center;
	std::vector<Vec3> button_src;
	std::vector<Vec3> button_dst;
	std::vector<Vec2> button_size;
	std::vector<Vec2> button_uv;
	std::vector<unsigned short> button_index;
	 
	unsigned int button_number;

	STAR_INTERFACE Star_Interface;
	
	unsigned int vao_id;
	unsigned int vbo_id[4];

	std::vector<StarAnt<Vec3> > starAnt;

	bool showing;
	bool vanishing;

	void setDST(int num)
	{
        for (int a = 0; a < num; a++)
        {
            if (Star_Interface ==  INTERFACE_CIRCLE)
            {
//                starLOG("circle interface\n");
                float short_side = width > height ? height : width;
                float angle = (M_PI*2. / num) * a;
                button_dst.push_back(Vec3(cos(angle)*short_side*0.2, sin(angle)*short_side*0.2, -1.0)+Vec3(width*0.5,height*0.5,0.0));
            }
            else if( Star_Interface == INTERFACE_RECT_VERTICAL)
            {
//                starLOG("vertical interface\n");
//                float short_side = width > height ? height : width;
//                float angle = (M_PI*2. / num) * a;
                button_dst.push_back(button_center[0]-Vec2(0.0,button_size[0].y*1.0*a));
            }
            else
            {
//                starLOG("else interface\n");
                button_dst.push_back(button_center[a]);
            }
            
        }
	}

	StarButton(StarButtonDelegate* _delegate=NULL)
	{
		delegate = _delegate;

		vao_id = vbo_id[0] = vbo_id[1] = vbo_id[2] = vbo_id[3] = 0;
		button_number = 0;
        now_tick = 0.;
		vanishing = false;
		showing = false;
	
		button_position.clear();
		button_uv.clear();
		button_index.clear();

		button_size.clear();
		button_dst.clear();
		button_src.clear();
		button_center.clear();
	};

   // int texture_id;

	StarButton* setInterface(STAR_INTERFACE interface)
	{
		Star_Interface = interface;
		return this;
	}

	StarButton* addButton(Vec4 rect)
	{
		button_position.push_back(Vec3(rect.x,rect.y,-1.));
		button_position.push_back(Vec3(rect.x+rect.z,rect.y,-1.));
		button_position.push_back(Vec3(rect.x,rect.y+rect.w,-1.));
		button_position.push_back(Vec3(rect.x+rect.z,rect.y+rect.w,-1.));

		button_size.push_back(Vec2(rect.z, rect.w));

		Vec3 temp = 
		(Vec3(rect.x, rect.y, -1.)
			+ Vec3(rect.x + rect.z, rect.y, -1.)
			+ Vec3(rect.x, rect.y + rect.w, -1.)
			+ Vec3(rect.x + rect.z, rect.y + rect.w, -1.))*0.25;

		button_src.push_back(temp);
		button_center.push_back(temp);

		StarAnt<Vec3> star;
		starAnt.push_back(star);
		//	push_back(temp);
		
		return this;
	}

	StarButton* addUV(Vec4 rect)
	{
		button_uv.push_back(Vec2(rect.x,rect.y+rect.w));
		button_uv.push_back(Vec2(rect.x+rect.z,rect.y+rect.w));
		button_uv.push_back(Vec2(rect.x,rect.y));
		button_uv.push_back(Vec2(rect.x+rect.z,rect.y));

		return this;
	}
    
    StarButton* setRect(Vec4 rect,int idx=0)
    {
        button_position[idx*4] = (Vec3(rect.x,rect.y,-1.));
        button_position[idx*4+1] = (Vec3(rect.x+rect.z,rect.y,-1.));
        button_position[idx*4+2] = (Vec3(rect.x,rect.y+rect.w,-1.));
        button_position[idx*4+3] = (Vec3(rect.x+rect.z,rect.y+rect.w,-1.));
        
        button_size[idx] = (Vec2(rect.z, rect.w));
        
        Vec3 temp =
        (Vec3(rect.x, rect.y, -1.)
         + Vec3(rect.x + rect.z, rect.y, -1.)
         + Vec3(rect.x, rect.y + rect.w, -1.)
         + Vec3(rect.x + rect.z, rect.y + rect.w, -1.))*0.25;
        
        button_src[idx] = (temp);
        button_dst[idx] = (temp);
        button_center[idx] =(temp);
        
//        starAnt[idx] = (star);
        return this;
    }
    //void initialize( int _vao_id, int _vbo_id)
	void init()
	{
			 //starAnt = new StarAnt<Vec3>[5];

		/*
		 * this part is defined right before it is called
		 */
        
//        starfbo->createFBO(true,true,fbo_width,fbo_height,_fbo_id);
//        
//        int err;
//        while ((err = glGetError()) != GL_NO_ERROR) {
//            printf("\n\nOpenGL error TURNON 1: %x\n\n",err);
//        }
//        GLenum status1 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//        if (status1 != GL_FRAMEBUFFER_COMPLETE) {
//            starLOG("ERROR FBO %x\n",status1);
//        }

		// butotn number
		button_number = button_position.size() / 4;
		if (!button_number)
		{
//			starLOG("Error, add buttton first\n");
			return;
		}
		else
		{
//			starLOG("Yes, buttton num : %d\n", button_number);
		}

		setDST(button_number);
        starfbo->createVAO_INDI(&vao_id);
        
#if(MAC|_WIN32)
        attribute_id[0] = 0;
        attribute_id[1] = 1;
        attribute_id[2] = 2;
#else
        attribute_id[0] = glGetAttribLocation(shader_program, "position");
        attribute_id[1] = glGetAttribLocation(shader_program, "uv");
//        attribute_id[2] = glGetAttribLocation(shader_program, "factor");
#endif

//        starfbo->createVBO_INDI(GL_ARRAY_BUFFER, sizeof(Vec3)*button_position.size(), (void*)&button_position[0], GL_STATIC_DRAW, &vbo_id[0]);
//        glEnableVertexAttribArray(attribute_id[0]);
//        glVertexAttribPointer(attribute_id[0], 3, GL_FLOAT, 0, 0, 0);
        //
        //starfbo->createVBO_INDI(GL_ARRAY_BUFFER, sizeof(Vec2)*button_uv.size(), (void*)&button_uv[0], GL_STATIC_DRAW, &vbo_id[1]);
        //glEnableVertexAttribArray(attribute_id[1]);
        //glVertexAttribPointer(attribute_id[1], 2, GL_FLOAT, 0, 0, 0);
        //

//        starLOG("test %d\n", vbo_id[0]);
		starfbo->createVBOsub_INDI(GL_ARRAY_BUFFER, 0, sizeof(Vec3)*button_position.size(), (void*)&button_position[0], GL_DYNAMIC_DRAW, &vbo_id[0]);
		glEnableVertexAttribArray(attribute_id[0]);
		glVertexAttribPointer(attribute_id[0], 3, GL_FLOAT, 0, 0, 0);
        

		starfbo->createVBO_INDI(GL_ARRAY_BUFFER, sizeof(Vec2)*button_uv.size(), (void*)&button_uv[0], GL_STATIC_DRAW, &vbo_id[1]);
		glEnableVertexAttribArray(attribute_id[1]);
		glVertexAttribPointer(attribute_id[1], 2, GL_FLOAT, 0, 0, 0);

	/*	starfbo->createVBOsub_INDI(GL_ARRAY_BUFFER, 0, sizeof(Vec2)*button_uv.size(), (void*)&button_uv[0], GL_DYNAMIC_DRAW, &vbo_id[1]);
		glEnableVertexAttribArray(attribute_id[1]);
		glVertexAttribPointer(attribute_id[1], 2, GL_FLOAT, 0, 0, 0);
*/
        
		for (int i = 0; i < button_number; i++)
		{
			for (int a = 0; a < 6; a++)
				button_index.push_back(rect_Idx_Vertex[a] + 4 * i);
		}
		
        starfbo->createVBO_INDI(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*button_index.size(), (void*)&button_index[0], GL_STATIC_DRAW, &vbo_id[2]);
        Matrix_Identity(final_matrix);
		Matrix_OrthoProjection(final_matrix, width, height, 0.1,1000.);
		Matrix a;
		Matrix_Translation(a, width*-0.5, height*-0.5, 0.0);
		Matrix_MxM(final_matrix, a ,final_matrix);

		/*
		* for test
		*/
		show();
    }
    
	void show()
	{
	//	starLOG("test show\n");
		for (int i = 0; i < button_number; i++)
		{
			starAnt[i].setSrcDst(&(button_center[i]), button_dst[i],2000.);

			//starAnt[i].setting_time = 2000.;
			//starAnt[i].remaining_time = 2000.;
			//starAnt[i].src_position = &(button_center[i]);
			//starAnt[i].dst_position = button_dst[i];

			//(&button_center[i], button_dst[i],2000.);
		}
	}
	void vanish()
	{
		for (int i = 0; i < button_number; i++)
		{
			starAnt[i].setSrcDst(&button_center[i], button_src[i], 2000.);
		}
	}
	
	void update()
	{

		for (int i = 0; i < button_number; i++)
		{
//			starLOG(" hmm now_tick %f\n?? ", now_tick);
			starAnt[i].work(now_tick);
		}

		for (int i = 0; i < button_number; i++)
		{
			button_position[i * 4 + 0] = button_center[i] - Vec3(button_size[i] * 0.5,0.0);
			button_position[i * 4 + 1] = button_center[i] + Vec3(Vec2(button_size[i].x, -button_size[i].y)*0.5,0.0);
			button_position[i * 4 + 2] = button_center[i] + Vec3(Vec2(-button_size[i].x, button_size[i].y)*0.5,0.0);
			button_position[i * 4 + 3] = button_center[i] + Vec3(button_size[i] * 0.5, 0.0);
		}
	}


    void render()
    {
        starfbo->bindVAO_INDI(&vao_id);
        
        glViewport(0,0, width,height);
        
        glUseProgram(shader_program);
        
        startexture->bindTEXTURE(GL_TEXTURE0+texture_name[0], texture_name[0]);
        texture_id[0] = glGetUniformLocation(shader_program, "texture0");
        glUniform1i(texture_id[0],texture_name[0]);
        
//        startexture->bindTEXTURE(GL_TEXTURE0+TEXTURES_BT, TEXTURES_BT);
//        texture_id[0] = glGetUniformLocation(shader_program, "texture0");
//        glUniform1i(texture_id[0],TEXTURES_BT);
        
       int a =  glGetUniformLocation(shader_program,"finalM");
        glUniformMatrix4fv(a, 1, GL_FALSE, final_matrix.s);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
//        glDisable(GL_BLEND);
		starfbo->bindVBO_INDI(GL_ARRAY_BUFFER, &vbo_id[0]);
        
		glEnableVertexAttribArray(attribute_id[0]);
		glVertexAttribPointer(attribute_id[0], 3, GL_FLOAT, 0, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*button_position.size(), (void*)&button_position[0], GL_DYNAMIC_DRAW);

      
#ifdef ANDROID
//        starfbo->bindVBO(GL_ARRAY_BUFFER,vbo_id[0]);
//        glVertexAttribPointer(attribute_id[0], 3, GL_FLOAT, 0, 0, 0);
//        glEnableVertexAttribArray(attribute_id[0]);
        
        starfbo->bindVBO_INDI(GL_ARRAY_BUFFER,&vbo_id[1]);
        glVertexAttribPointer(attribute_id[1],2, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(attribute_id[1]);
        
        starfbo->bindVBO_INDI(GL_ELEMENT_ARRAY_BUFFER, &vbo_id[2]);
#endif

        glDrawElements(GL_TRIANGLES,button_index.size(), GL_UNSIGNED_SHORT ,(void*)0);
        
//        while ((err = glGetError()) != GL_NO_ERROR) {
//            printf("\n\nOpenGL error Button Irender error: %x\n\n",err);
//        }
        
		glDisable(GL_BLEND);
    }

void done()
{
	button_position.clear();
	button_uv.clear();
	button_index.clear();
}

/*
* Callback Functions (sending)
*/
void pressed(int idx)
{
    starLOG("oh pressed %d\n", idx);
    
    if(delegate)
	delegate->Callback_Press(idx);
}

/*
 *  Callback Functions  (receiving)
 */
void CallbackFPS() 
{
	
};

bool test(Vec3* center,Vec2* size, Vec2* touch)
{

	//if ((rect->x < touch->x) && (rect->x + rect->z > touch->x))
	//	if ((rect->y < touch->y) && (rect->y + rect->w > touch->y))
	//		return true;
    
//    starLOG("touch %f %f\n", touch->x, touch->y);
//    starLOG("center %f %f\n", center->x, center->y);
//    starLOG("diff %f", abs(center->x - touch->x));
//    starLOG(" %f\n", abs(center->y - touch->y));
//    starLOG("size %f %f\n", size->x*0.5, size->y*0.5);
    
	if ( abs((int)(center->x - touch->x)) < (size->x*0.5))
		if (abs((int)(center->y - touch->y)) < (size->y*0.5))
			return true;
	return false;
}
void CallbackTouchBegin()
{
//starLOG("xy %f %f\n", startouch->nowPos[0].x, startouch->nowPos[0].y);
	for (int i = 0; i < button_number; i++)
	{
			//Vec4 rect = Vec4(
			//(Vec2(button_center[i].x,button_center[i].y) - button_size[i] * 0.5).x
			//(Vec2(button_center[i].x, button_center[i].y) - button_size[i] * 0.5).y
			//	, button_size[i].x, button_size[i].y);

			if (test(&button_center[i], &button_size[i], &(startouch->nowPos[0])))
			{
                starLOG("pressed\n");
				pressed(i);
               // vanish();
                //showing = true;
				break;
			}
            else
            {
                //show();
                //showing = false;
            }
	}

//	if (showing)
//	{
//		show();
//        showing = false;
//	}
////	else
//    if(!showing)
//	{
//		
//	}

};

void CallbackTouchMove() 
{

};

void CallbackTouchEnd() 
{

};

void CallbackTouchCancel() 
{

};

};
#endif
