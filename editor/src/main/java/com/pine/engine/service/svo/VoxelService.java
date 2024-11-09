package com.pine.engine.service.svo;

import com.pine.impl.Loggable;
import com.pine.engine.injection.PBean;
import com.pine.engine.injection.PInject;
import com.pine.engine.repository.core.CoreSSBORepository;
import com.pine.engine.repository.voxelization.VoxelRepository;
import com.pine.engine.service.resource.SSBOService;
import com.pine.engine.tasks.SyncTask;
import org.lwjgl.system.MemoryUtil;

import java.util.HashMap;
import java.util.Map;

@PBean
public class VoxelService implements SyncTask, Loggable {

    @PInject
    public VoxelRepository voxelRepository;

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
        dataByIndex.clear();
        currentOctreeMemIndex = 0;
        long startTotal = System.currentTimeMillis();
        SparseVoxelOctree octree = new SparseVoxelOctree(
                voxelRepository.gridScale,
                voxelRepository.maxDepth,
                voxelRepository.voxelizationStepSize
        );
        LorenzAttractorDemo.fill(octree);

        long startMemory = System.currentTimeMillis();
        voxelRepository.voxels = octree.buildBuffer();
        getLogger().warn("Voxel buffer creation took {}ms", System.currentTimeMillis() - startMemory);

        needsPackaging = true;
        getLogger().warn("Total voxelization time {}ms", System.currentTimeMillis() - startTotal);
    }

    private void packageData() {
        int sizeInBits = voxelRepository.voxels.length * 32;
        getLogger().warn("Node quantity {}", voxelRepository.voxels.length);
        getLogger().warn("Buffer size {}bits {}bytes {}mb", sizeInBits, (sizeInBits / 8), sizeInBits / 8_000_000);

        var octreeMemBuffer = MemoryUtil.memAllocInt(voxelRepository.voxels.length);
        int[] voxels = voxelRepository.voxels;
        for (int i = 0, voxelsLength = voxels.length; i < voxelsLength; i++) {
            int voxelData = voxels[i];
            octreeMemBuffer.put(i, voxelData);
        }
        ssboService.updateBuffer(coreSSBORepository.octreeSSBO, octreeMemBuffer, 0);
        MemoryUtil.memFree(octreeMemBuffer);
    }
}
