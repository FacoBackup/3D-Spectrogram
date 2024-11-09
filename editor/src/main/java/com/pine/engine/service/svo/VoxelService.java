package com.pine.engine.service.svo;

import com.pine.engine.service.resource.ResourceService;
import com.pine.engine.service.resource.shader.GLSLType;
import com.pine.engine.service.resource.ssbo.SSBOCreationData;
import com.pine.engine.service.resource.ssbo.ShaderStorageBufferObject;
import com.pine.impl.Loggable;
import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.core.CoreSSBORepository;
import com.pine.engine.repository.voxelization.VoxelRepository;
import com.pine.engine.service.resource.SSBOService;
import com.pine.engine.tasks.SyncTask;
import org.joml.Vector3f;
import org.lwjgl.system.MemoryUtil;

import java.util.HashMap;
import java.util.Map;

import static java.lang.Math.sin;

@PBean
public class VoxelService implements SyncTask, Loggable {

    @PInject
    public VoxelRepository voxelRepository;

    @PInject
    public ResourceService resources;

    @PInject
    public CoreSSBORepository coreSSBORepository;

    @PInject
    public SSBOService ssboService;


    private boolean needsPackaging = true;
    private final Map<VoxelData, Integer> dataByIndex = new HashMap<>();
    private int currentOctreeMemIndex = 0;

    @Override
    public void sync() {
        if (voxelRepository.voxels == null || !needsPackaging) {
            return;
        }
        packageData();
        needsPackaging = false;
    }

    public void buildFromScratch() {
        new Thread(() -> {
            dataByIndex.clear();
            currentOctreeMemIndex = 0;
            long startTotal = System.currentTimeMillis();
            SparseVoxelOctree octree = new SparseVoxelOctree(
                    voxelRepository.gridScale,
                    voxelRepository.maxDepth,
                    voxelRepository.voxelizationStepSize
            );
//        LorenzAttractorDemo.fill(octree);
            float multiplier = voxelRepository.voxelizationStepSize;
            int count = 1000;
            for (int i = -count; i < count; i++) {
                for (int j = -count; j < count; j++) {
                    octree.insert(new Vector3f(
                            i * multiplier + octree.getOffset(),
                            (float) (wavePattern(i * multiplier, j * multiplier)) + octree.getOffset() + 1,
                            j * multiplier + octree.getOffset()
                    ), new VoxelData(1, 1, 1));
                }
            }

            long startMemory = System.currentTimeMillis();
            voxelRepository.voxels = octree.buildBuffer();
            getLogger().warn("Voxel buffer creation took {}ms", System.currentTimeMillis() - startMemory);

            needsPackaging = true;
            getLogger().warn("Total voxelization time {}ms", System.currentTimeMillis() - startTotal);
        }).start();
    }

    private float wavePattern(float i, float j) {
        float frequency1 = 1.0f;
        float frequency2 = 2.5f;
        float frequency3 = 5.0f;
        float amplitude1 = 0.3f;
        float amplitude2 = 0.15f;
        float amplitude3 = 0.1f;

        float wave = 0.0f;
        wave += (float) (amplitude1 * sin(frequency1 * i + j));
        wave += (float) (amplitude2 * sin(frequency2 * i - j * 0.8));
        wave += (float) (amplitude3 * sin(frequency3 * i + j * 1.2));

        return wave;
    }

    private void packageData() {
        int sizeInBits = voxelRepository.voxels.length * 32;
        getLogger().warn("Voxel quantity: {} | Buffer size: {}bits {}bytes {}mb", voxelRepository.voxels.length, sizeInBits, (sizeInBits / 8), sizeInBits / 8_000_000);

        var octreeMemBuffer = MemoryUtil.memAllocInt(voxelRepository.voxels.length);
        int[] voxels = voxelRepository.voxels;
        for (int i = 0, voxelsLength = voxels.length; i < voxelsLength; i++) {
            int voxelData = voxels[i];
            octreeMemBuffer.put(i, voxelData);
        }
        if (coreSSBORepository.octreeSSBO != null) {
            coreSSBORepository.octreeSSBO.dispose();
        }
        coreSSBORepository.octreeSSBO = (ShaderStorageBufferObject) resources.addResource(new SSBOCreationData(
                12,
                (long) voxels.length * GLSLType.INT.getSize()
        ));
        ssboService.updateBuffer(coreSSBORepository.octreeSSBO, octreeMemBuffer, 0);
        MemoryUtil.memFree(octreeMemBuffer);
    }
}
