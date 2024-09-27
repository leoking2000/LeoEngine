#include "AssetManager.h"
#include "../Utilities/FileUtilities.h"
#include "stb/stb_image.h"

namespace LEO
{
    AssetManager::~AssetManager()
    {
        m_mesh_map.clear();
        m_texture_map.clear();
        m_shader_map.clear();
        m_model_map.clear();
    }


    u32 AssetManager::AddMesh(DefaultMesh shape)
    {
        auto inserted = m_mesh_map.insert(std::make_pair(m_insertedAssetID, Mesh{shape}));
        m_insertedAssetID++;

        return m_insertedAssetID - 1;
    }

    void AssetManager::RemoveMesh(u32 id)
    {
        Remove<Mesh>(m_mesh_map, id, "Mesh");
    }

    Mesh& AssetManager::GetMesh(u32 id)
    {
        return Get<Mesh>(m_mesh_map, id, "Mesh");
    }



    u32 AssetManager::AddModel()
    {
        auto inserted = m_model_map.insert(std::make_pair(m_insertedAssetID, Model(*this)));
        m_insertedAssetID++;

        return m_insertedAssetID - 1;
    }

    //u32 AssetManager::AddModel(const std::string& filepath)
    //{
    //    std::vector<CPUMesh> cpu_model = CPUMesh::Load(filepath, *this);

    //    u32 model_id = AddModel();
    //    Model& model = GetModel(model_id);

    //    for (CPUMesh& m : cpu_model)
    //    {
    //        // add the mesh
    //        auto inserted = m_mesh_map.insert(std::make_pair(m_insertedAssetID, CPUMesh::Convert(m)));
    //        m_insertedAssetID++;
    //        // add the sub model to the model
    //        model.AddSubModel({ m_insertedAssetID - 1, m.material });
    //    }

    //    return model_id;
    //}

    u32 AssetManager::AddModel(u32 mesh, Material mat)
    {
        SubModel sub_model{ mesh, mat };
        auto inserted = m_model_map.insert(std::make_pair(m_insertedAssetID, Model(sub_model, *this)));
        m_insertedAssetID++;

        return m_insertedAssetID - 1;
    }

    void AssetManager::RemoveModel(u32 id)
    {
        Remove<Model>(m_model_map, id, "Model");
    }

    Model& AssetManager::GetModel(u32 id)
    {
        return Get<Model>(m_model_map, id, "Model");
    }



    u32 AssetManager::AddTextureFromFile(const std::string& filepath)
    {
        stbi_set_flip_vertically_on_load(1);

        int width;
        int height;
        int bpp;

        stbi_uc* data = stbi_load((filepath).c_str(), &width, &height, &bpp, 4);
        bool fail = false;

        if (data == nullptr)
        {
            LOGERROR("Failed to Created Texture From: {}", filepath);
            stbi_uc data_error[] = {
                    255, 255, 255, 255, /**/  0,   0,   0, 255,
                      0,   0,   0, 255,     255, 255, 255, 255
            };
            width = 2;
            height = 2;
            data = data_error;
            fail = true;
        }

        Texture tex(DIM_2D, { width, height, 0 }, TextureFormat::RGBA8UB,
            TextureMinFiltering::MIN_LINEAR_MIPMAP_LINEAR, TextureMagFiltering::MAG_LINEAR,
            TextureWrapping::REPEAT, TextureWrapping::REPEAT,
            data
        );
        if (!fail) { stbi_image_free(data); };

        auto inserted = m_texture_map.insert(std::make_pair(m_insertedAssetID, std::move(tex)));
        m_insertedAssetID++;

        return m_insertedAssetID - 1;
    }

    void AssetManager::RemoveTexture(u32 id)
    {
        Remove<Texture>(m_texture_map, id, "Texture");
    }

    Texture& AssetManager::GetTexture(u32 id)
    {
        return Get<Texture>(m_texture_map, id, "Texture");
    }



    u32 AssetManager::AddShader(const std::string& filepath)
    {
        auto inserted = m_shader_map.insert(std::make_pair(m_insertedAssetID, ShaderProgram(filepath)));

        m_insertedAssetID++;

        return m_insertedAssetID - 1;
    }

    void AssetManager::RemoveShader(u32 id)
    {
        Remove<ShaderProgram>(m_shader_map, id, "Shader");
    }

    ShaderProgram& AssetManager::GetShader(u32 id)
    {
        return Get<ShaderProgram>(m_shader_map, id, "Shader");
    }
}