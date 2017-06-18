# include "../../include/keyframes/KeyframeCollection.hpp"
# include <glm/gtx/compatibility.hpp>

void KeyframeCollection::add(float time, const GeometricTransformation& transformation)
{
    m_keyframes.insert(std::make_pair(time, transformation));
}

glm::mat4 KeyframeCollection::interpolateTransformation(float time) const
{
    if (!m_keyframes.empty()) {
        //Handle the case where the time parameter is outside the keyframes time scope.
        std::map<float, GeometricTransformation>::const_iterator itFirstFrame = m_keyframes.begin();
        std::map<float, GeometricTransformation>::const_reverse_iterator itLastFrame = m_keyframes.rbegin();
        if (time <= itFirstFrame->first)
            return itFirstFrame->second.toMatrix();
        if (time >= itLastFrame->first)
            return itLastFrame->second.toMatrix();

        //Get keyframes surrounding the time parameter
        std::array<Keyframe, 2> result = getBoundingKeyframes(time);
        float time0 = result[0].first;
        float time1 = result[1].first;
        GeometricTransformation transform0 = result[0].second;
        GeometricTransformation transform1 = result[1].second;

        float factor = (time - time0) / (time1 - time0);

        glm::quat orientation = glm::slerp(transform0.getOrientation(), transform1.getOrientation(), factor);
        
        
        glm::vec3 t0 = transform0.getTranslation();
        glm::vec3 t1 = transform1.getTranslation();
        glm::vec3 translation;
        
        if (t0.z - t1.z < -8.0) { // dans la cas où l'objet remonte suivant z,
                                // on n'interpole pas cette montée pour éviter 
                                // que les flocons rebondissent sur le sol
            translation = glm::lerp(transform1.getTranslation(), transform1.getTranslation(), factor);
        } else { // gestion classique
            translation = glm::lerp(transform0.getTranslation(), transform1.getTranslation(), factor);
        }
        
        glm::vec3 scale = glm::lerp(transform0.getScale(), transform1.getScale(), factor);
        return GeometricTransformation(translation, orientation, scale).toMatrix();
        
    } else {
        return glm::mat4(1.0);
    }
}

bool KeyframeCollection::empty() const
{
    return m_keyframes.empty();
}

std::array< KeyframeCollection::Keyframe, 2 > KeyframeCollection::getBoundingKeyframes(float time) const
{
    std::array<KeyframeCollection::Keyframe, 2> result {
        std::make_pair(0, GeometricTransformation()),
        std::make_pair(0, GeometricTransformation()) };
    std::map<float, GeometricTransformation>::const_iterator upper = m_keyframes.upper_bound(time);
    std::map<float, GeometricTransformation>::const_iterator lower = std::prev(upper);
    std::map<float, GeometricTransformation>::const_iterator end = m_keyframes.end();
    if (upper != end && lower != end) {
        result[0] = *lower;
        result[1] = *upper;
    }
    return result;
}
