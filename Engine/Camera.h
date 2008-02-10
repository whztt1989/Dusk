#ifndef CAMERA_H
#define CAMERA_H

#include <Ogre.h>

namespace Dusk
{
    class Camera
    {
        public:
            Camera(Ogre::SceneManager* scn);
            virtual ~Camera();
            Ogre::Camera* getOgreCamera();
            void setPosition(const Ogre::Vector3& position);
            void lookAt(const Ogre::Vector3& direction);
        protected:
        private:
            Ogre::Camera* m_Camera;
            Ogre::SceneNode* m_Primary;
            Ogre::SceneNode* m_Secondary;
    };
}

#endif // CAMERA_H