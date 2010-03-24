/*---------------------------------------------------------------------------
 Authors: walljumper, thoronador
 Date:    2010-03-24
 Purpose: Camera class
          Manages the game's camera.

 History:
     - 2008-02-08 (rev 34)  - initial version (by walljumper)
     - 2008-03-22 (rev 51)  - movement/ translation functions added,
                            - frame listener calls move()
     - 2008-03-25 (rev 55)  - move() and translate() actually implemented
     - 2009-06-21 (rev 98)  - rotate() added, camera rotation implemented
                              (by thoronador)
     - 2009-06-30 (rev 100) - camera adjusts its position to move along
                              landscape, if landscape data is present
     - 2009-12-04 (rev 138) - zoom functionality added
     - 2009-12-31 (rev 147) - documentation update
     - 2010-03-24 (rev 186) - singleton pattern; jump() added

 ToDo list:
     - ???
 Bugs:
     - No known bugs. If you find one (or more), then tell us please.
 --------------------------------------------------------------------------*/

#ifndef CAMERA_H
#define CAMERA_H

#include <Ogre.h>

namespace Dusk
{
    class Camera
    {
        public:
            static Camera& getSingleton();
            virtual ~Camera();

            /* initializes Camera for use by application

               remarks:
                  Has to be called once before the Camera is used.
             */
            void setupCamera(Ogre::SceneManager* scn);

            /* retrieves the pointer to the Ogre::Camera object which is
               internally used*/
            Ogre::Camera* getOgreCamera();

            /* sets the position of the camera

               remarks:
                   The vector does not neccessarily represent the actual
                   position of the camera object but only the position of the
                   camera before zoom is applied.
            */
            void setPosition(const Ogre::Vector3& position);

            /* Tells the camera where to look at. */
            void lookAt(const Ogre::Vector3& direction);

            /* Performs movement of the camera, i.e. translation and rotation,
               based on the time given in evt.timeSinceLastFrame. */
            void move(const Ogre::FrameEvent& evt);

            /* Adds(!) to the internally used translation vector. */
            void translate(const Ogre::Vector3& translationVector);

            /* Adds(!) to the internally used rotation value.

               remarks:
                   The value is interpreted as degrees, not as radians.
            */
            void rotate(const float rotation);

            /* tells the Camera to "jump" upwards

               remarks:
                  Has no effect, if the camera already performs a jump.
            */
            void jump(void);

            /* Sets the zoom distance.

               remarks:
                   The distance is interpreted in world units behind the camera
                   position specified with setPosition(), i.e. greater values
                   will get the viewpoint further away from the camera position,
                   while a value of zero means you are exactly at the camera
                   position.
                   Note that the distance will be capped if outside of the range
                   [cMinimumZoom,cMaximumZoom].
            */
            void setZoom(const float distance);

            /* Returns the current zoom distance. */
            float getZoom() const;

            /* constants to limit zoom range in setZoom() */
            static const float cMinimumZoom;
            static const float cMaximumZoom;

            /* constant to indicate change in zoom for one keystroke/ one turn
               of mouse wheel (recommended)*/
            static const float cRecommendedZoomStep;
        protected:
        private:
            Camera(Ogre::SceneManager* scn=NULL);
            Camera(const Camera& op) {}
            Ogre::Camera* m_Camera;
            Ogre::SceneNode* m_Primary;
            Ogre::SceneNode* m_Secondary;
            Ogre::Vector3 m_translationVector;
            float m_RotationPerSecond;

            //data for jumping (I know, not the Camera but the player should
            // jump, but we don't have a player object yet.)
            float m_JumpVelocity;
            bool m_Jump;

            /* default distance between camera and ground/ landscape */
            static const float cAboveGroundLevel;
    };
}

#endif // CAMERA_H
