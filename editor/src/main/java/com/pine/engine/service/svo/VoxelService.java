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
import com.pine.impl.NativeDialogService;
import org.joml.Vector3f;
import org.lwjgl.system.MemoryUtil;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.util.HashMap;
import java.util.Map;

@PBean
public class VoxelService implements SyncTask, Loggable {

    @PInject
    public VoxelRepository voxelRepository;

    @PInject
    public ResourceService resources;

    @PInject
    public CoreSSBORepository coreSSBORepository;

    @PInject
    public NativeDialogService nativeDialogService;

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

        var file = nativeDialogService.selectFile().stream().findFirst().orElse(null);
        try {
            // Load the audio file
            File audioFile = new File(file);
            AudioInputStream audioStream = AudioSystem.getAudioInputStream(new BufferedInputStream(new FileInputStream(audioFile)));

            // Get the format of the audio data
            AudioFormat format = audioStream.getFormat();
            System.out.println("Sample Rate: " + format.getSampleRate());
            System.out.println("Channels: " + format.getChannels());

            // Convert the audio data into bytes
            byte[] audioBytes = audioStream.readAllBytes();

            // Process the audio data
            float sampleRate = format.getSampleRate();
            int bytesPerSample = format.getSampleSizeInBits() / 8;

            new Thread(() -> {
                try {
                    dataByIndex.clear();
                    currentOctreeMemIndex = 0;
                    long startTotal = System.currentTimeMillis();
                    SparseVoxelOctree octree = new SparseVoxelOctree(
                            voxelRepository.gridScale,
                            voxelRepository.maxDepth,
                            voxelRepository.voxelizationStepSize
                    );


                    int bytesPerFrame = format.getFrameSize();
                    int totalFrames = audioBytes.length / bytesPerFrame;
                    double[] timestamps = new double[totalFrames];
                    double[] amplitudes = new double[totalFrames];
                    for (int i = 0; i < totalFrames; i++) {
                        int sampleIndex = i * bytesPerFrame;
                        int sampleValue = 0;
                        if (format.getSampleSizeInBits() == 16) {
                            sampleValue = ((audioBytes[sampleIndex + 1] << 8) | (audioBytes[sampleIndex] & 0xff));
                        } else if (format.getSampleSizeInBits() == 8) {
                            sampleValue = audioBytes[sampleIndex];
                        }

                        amplitudes[i] = sampleValue / Math.pow(2, format.getSampleSizeInBits() - 1);
                        timestamps[i] = i / sampleRate;
                    }


                    generatePoints(octree, timestamps, amplitudes);
//
//                    float multiplier = voxelRepository.voxelizationStepSize;
//                    final float MIN_AMPLITUDE = 100;
//                    final float MIN_TIME = audioBytes.length / 2f;
//                    getLogger().warn("Samples: {} | bytes per sample: {} | sample rate {}", samplesPerSecond, bytesPerSample, sampleRate);
//                    for (int i = 0; i < audioBytes.length; i += bytesPerSample * numChannels) {
//                        for (int j = 0; j < bytesPerSample; j++) {
//                            var sample = (audioBytes[i + j] & 0xFF) << (8 * j);
//
//                            octree.insert(new Vector3f(
//                                    ((MIN_TIME - i - j * numChannels) * multiplier) + octree.getOffset(),
//                                    (MIN_AMPLITUDE - sample) * multiplier + octree.getOffset(),
//                                    octree.getOffset()
//                            ), new VoxelData(1, 1, 1));
//                        }
//                    }

                    audioStream.close();

                    long startMemory = System.currentTimeMillis();
                    voxelRepository.voxels = octree.buildBuffer();
                    getLogger().warn("Voxel buffer creation took {}ms", System.currentTimeMillis() - startMemory);

                    needsPackaging = true;
                    getLogger().warn("Total voxelization time {}ms", System.currentTimeMillis() - startTotal);
                } catch (Exception e) {
                    getLogger().error(e.getMessage(), e);
                }
            }).start();
        } catch (Exception e) {
            getLogger().error(e.getMessage(), e);
        }
    }

    public void generatePoints(SparseVoxelOctree octree, double[] times, double[] amplitudes) {
        float maxTime = (float) times[times.length - 1]/2;
        for (int i = 0; i < times.length; i++) {
            float x = (float) times[i];
            float y = (float) amplitudes[i];
            octree.insert(new Vector3f(
                    octree.getOffset() - x * voxelRepository.xScale + maxTime,
                    octree.getOffset() - y,
                    octree.getOffset()
            ), new VoxelData(1, 1, 1));
            if(i == times.length -1){
                getLogger().warn("Final timestamp {}", x);
            }
        }
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
